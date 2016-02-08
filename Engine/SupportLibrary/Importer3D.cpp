#include "Importer3D.h"

// Assimp
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"
#include "assimp/Scene.h"

// Util
#include "PlatformLibrary/Sys.h"
#include <fstream>
#include <map>
#include <unordered_map>
#include <functional>

bool Importer3D::LoadModelFromFile(const std::string& path, const rImporter3DCfg& cfg, rImporter3DData& data_out) {
	Assimp::Importer importer;

	std::ifstream is(path, std::ios::ate);
	if (!is.is_open()) {
		// TODO
		return false;
	}

	// Read file into memory
	u32 fileSize = (u32)is.tellg();
	void* mem = malloc(fileSize);
	is.seekg(0, std::ios::beg);
	is.read((char*)mem, fileSize);
	is.close();

	// Assimp will parse memory
	const aiScene* scene = importer.ReadFileFromMemory(mem, fileSize, aiProcess_MakeLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	// Free memory
	free(mem);

	if (!scene)
		return false;


	// Gather absolute transformation matrices for each mesh, we will transform vertices with that, to get final positions
	std::vector<aiMatrix4x4> meshTransformations(scene->mNumMeshes);


	//Node000RelTrans * Node00RelTrans * Node0RelTrans * rootRelTrans
	// Root
	// -Node 0
	//  - Node 00
	//	  - Node 000
	//  - Node 01
	// -Node 1
	//  - Node 10
	//	   - Node 100

	std::function<void(aiNode*, aiMatrix4x4 absoluteTransform)> CalcWorldMatrixRecursively;
	CalcWorldMatrixRecursively = [&](aiNode* node, aiMatrix4x4 absoluteTransform)
	{
		absoluteTransform = node->mTransformation * absoluteTransform;

		if (node->mNumMeshes > 0)
		{
			for (size_t i = 0; i < node->mNumMeshes; i++)
			{
				unsigned int meshIdx = node->mMeshes[i];
				meshTransformations[meshIdx] = absoluteTransform;
			}
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
			CalcWorldMatrixRecursively(node->mChildren[i], absoluteTransform);
	};

	aiMatrix4x4 identityMat;
	CalcWorldMatrixRecursively(scene->mRootNode, identityMat);

	// Parsed "scene" have meshes
	size_t nMeshes = scene->mNumMeshes;
	aiMesh** meshes = scene->mMeshes;

	u32 nIndices = 0;
	//nMatGroups = nMeshes; // Assimp breaks material groups into N meshes

	size_t nVertices = 0;

	// Get indexCount, vertexCount, Gather matGroups from meshes
	std::vector<u32> matIDs;
	matIDs.resize(nMeshes);
	for (size_t i = 0; i < nMeshes; i++)
	{
		matIDs[i] = nIndices / 3;

		// VB, IB
		nVertices += meshes[i]->mNumVertices;
		nIndices += meshes[i]->mNumFaces * 3;
	}


	u32 vertexSize = 0; // Gather vertex Size
	u32 pos_attribOffset;
	u32 norm_attribOffset;
	u32 tan_attribOffset;
	u32 bitan_attribOffset;
	u32 tex0_attribOffset;
	u32 boneIndices_attribOffset;
	u32 boneWeights_attribOffset;

	u32 offset = 0;
	for (auto f : cfg.flags)
	{
		switch (f)
		{
		case eImporter3DFlag::VERT_ATTR_POS:			vertexSize += sizeof(mm::vec3);  pos_attribOffset = offset;			offset += sizeof(mm::vec3);  break;
		case eImporter3DFlag::VERT_ATTR_TEX0:			vertexSize += sizeof(mm::vec2);  tex0_attribOffset = offset;		offset += sizeof(mm::vec2);  break;
		case eImporter3DFlag::VERT_ATTR_NORM:			vertexSize += sizeof(mm::vec3);  norm_attribOffset = offset;		offset += sizeof(mm::vec3);  break;
		case eImporter3DFlag::VERT_ATTR_TAN:			vertexSize += sizeof(mm::vec3);  tan_attribOffset = offset;			offset += sizeof(mm::vec3);  break;
		case eImporter3DFlag::VERT_ATTR_BITAN:			vertexSize += sizeof(mm::vec3);  bitan_attribOffset = offset;		offset += sizeof(mm::vec3);  break;
		case eImporter3DFlag::VERT_ATTR_BONE_INDICES:	vertexSize += sizeof(u32) * 4;	 boneIndices_attribOffset = offset;	offset += sizeof(mm::vec2);	 break;
		case eImporter3DFlag::VERT_ATTR_BONE_WEIGHTS:	vertexSize += sizeof(float) * 4; boneWeights_attribOffset = offset;	offset += sizeof(float) * 4; break;
		}
	}

	// Copy indices, vertices from meshes
	void* vertices = new u8[vertexSize * nVertices];
	u32* indices = new u32[nIndices];

	// We collect each meshes indices and vertices into a big vertices, indices buffer, so meshes have local indexes to their local vertices
	// we have global index for containers.  GlobalIdx += localIdx
	u32 globalIndicesIdx = 0;
	u32 globalVertexIdx = 0;
	u32 vertexOffset = 0;

	// TODO: hack, will be combined to 1 mesh always, todo
	rImporter3DMesh* mesh_out = new rImporter3DMesh();
	mesh_out->materials.resize(nMeshes);

	// Each mesh
	for (size_t i = 0; i < nMeshes; i++)
	{
		aiMesh* mesh = meshes[i];

		// Get mesh material infos
		//rImporter3DMesh::rMaterial material;
		mesh_out->materials[i].faceStartIdx = globalIndicesIdx / 3;
		mesh_out->materials[i].faceEndIdx = globalIndicesIdx / 3 + mesh->mNumFaces;

		aiString diffusePath;
		aiString normalPath;
		bool bHaveDiffuseTexture = aiReturn_SUCCESS == scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath);
		bool bHaveNormalTexture = aiReturn_SUCCESS == scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_NORMALS, 0, &normalPath);


		// Calc absolute model path if we have any textures associated with that model
		std::string relModelPath;
		if (bHaveDiffuseTexture || bHaveNormalTexture)
		{
			std::string modelDirectory = path;
			auto chIdx = modelDirectory.rfind('/');
			modelDirectory = modelDirectory.substr(0, chIdx + 1);

			auto shit = GetAssetsPath();
			size_t offs = shit.length();
			relModelPath = modelDirectory.substr(offs, modelDirectory.length() - offs);
		}

		// Get Diffuse texture path relative to work dir
		if (bHaveDiffuseTexture)
		{
			size_t idx = 0;
			while (diffusePath.data[idx] == '.' || diffusePath.data[idx] == '\\' || diffusePath.data[idx] == '/')
				idx++;

			if (idx != 0)
			{
				memmove(diffusePath.data, diffusePath.data + idx, diffusePath.length - idx);
				diffusePath.data[diffusePath.length - idx] = 0;
			}

			mesh_out->materials[i].relTexPathDiffuse = relModelPath + diffusePath.C_Str();
		}


		// Get Normal texture path relative to work dir
		if (bHaveNormalTexture)
		{
			size_t idx = 0;
			while (normalPath.data[idx] == '.' || normalPath.data[idx] == '\\' || normalPath.data[idx] == '/')
				idx++;

			if (idx != 0)
			{
				memmove(normalPath.data, normalPath.data + idx, normalPath.length - idx);
				normalPath.data[normalPath.length - idx] = 0;
			}

			mesh_out->materials[i].relTexPathNormal = relModelPath + normalPath.C_Str();
		}

		bool bHasPos = mesh->HasPositions();
		bool bHasNormals = mesh->HasNormals();

		// Gather Each face
		for (size_t j = 0; j < mesh->mNumFaces; globalIndicesIdx += 3, j++)
		{
			aiFace& face = mesh->mFaces[j];

			// Each vertex on face
			for (size_t k = 0; k < 3; k++)
			{
				u32 localVertIdx = face.mIndices[k];

				// Gather Index data
				indices[globalIndicesIdx + k] = localVertIdx + globalVertexIdx;

				if (localVertIdx + globalVertexIdx >= nVertices)
				{
					assert(0);
				}

				// Gather position
				if (bHasPos & cfg.isContain(eImporter3DFlag::VERT_ATTR_POS))
				{
					const aiVector3D& pos = meshTransformations[i] * mesh->mVertices[localVertIdx];

					// Determine vertex index
					u32 vertexIdx = localVertIdx + globalVertexIdx;

					// Then copy the data to the appropriate attrib offset in that vertex
					memcpy(((u8*)vertices) + vertexSize * vertexIdx + pos_attribOffset, &mm::vec3(pos.x, pos.z, pos.y), sizeof(mm::vec3)); // z and y swapped !!
				}

				// Gather normal
				if (bHasNormals & cfg.isContain(eImporter3DFlag::VERT_ATTR_NORM))
				{
					const aiVector3D& normal = mesh->mNormals[localVertIdx];

					// Determine vertex index
					u32 vertexIdx = localVertIdx + globalVertexIdx;

					// Then copy the data to the appropriate attrib offset in that vertex
					memcpy(((u8*)vertices) + vertexSize * vertexIdx + norm_attribOffset, &mm::vec3(normal.x, normal.y, normal.z), sizeof(mm::vec3));
				}

				if (mesh->HasTangentsAndBitangents()) 
				{
					if(cfg.isContain(eImporter3DFlag::VERT_ATTR_TAN))
					{
						const aiVector3D& tangent = mesh->mTangents[localVertIdx];

						// Determine vertex index
						u32 vertexIdx = localVertIdx + globalVertexIdx;

						// Then copy the data to the appropriate attrib offset in that vertex
						memcpy(((u8*)vertices) + vertexSize * vertexIdx + tan_attribOffset, &mm::vec3(tangent.x, tangent.y, tangent.z), sizeof(mm::vec3));
					}

					if (cfg.isContain(eImporter3DFlag::VERT_ATTR_BITAN))
					{
						const aiVector3D& bitangent = mesh->mBitangents[localVertIdx];

						// Determine vertex index
						u32 vertexIdx = localVertIdx + globalVertexIdx;

						// Then copy the data to the appropriate attrib offset in that vertex
						memcpy(((u8*)vertices) + vertexSize * vertexIdx + bitan_attribOffset, &mm::vec3(bitangent.x, bitangent.y, bitangent.z), sizeof(mm::vec3));
					}
				}

				// @TODO not general algorithm, wee need to handle more UV channels
				auto vecPtr = mesh->mTextureCoords[0];
				if ((bool)vecPtr & cfg.isContain(eImporter3DFlag::VERT_ATTR_TEX0))
				{
					const aiVector3D& tex0 = vecPtr[localVertIdx];

					// Determine vertex index
					u32 vertexIdx = localVertIdx + globalVertexIdx;

					// Then copy the data to the appropriate attrib offset in that vertex
					//memcpy(((u8*)vertices) + vertexSize * vertexIdx + tex0_attribOffset, &mm::vec2(tex0.x, 1 - tex0.y), sizeof(mm::vec2)); // UV flip y
					memcpy(((u8*)vertices) + vertexSize * vertexIdx + tex0_attribOffset, &mm::vec2(tex0.x, tex0.y), sizeof(mm::vec2));
				}
			}
		}

		// TODO
		if (mesh->HasBones())
		{

		}
		globalVertexIdx += mesh->mNumVertices;
	}

	// If requested, recenter pivot
	if (cfg.isContain(eImporter3DFlag::PIVOT_RECENTER))
	{
		mm::vec3 avgCenter(0, 0, 0);
		for (u32 i = 0; i < nVertices; i++)
		{
			mm::vec3* pos = (mm::vec3*)(((u8*)vertices) + i * vertexSize);
			avgCenter += *pos;
		}
		avgCenter /= nVertices;

		for (u32 i = 0; i < nVertices; i++)
		{
			mm::vec3* pos = (mm::vec3*)(((u8*)vertices) + i * vertexSize);
			*pos -= avgCenter;
		}
	}

	mesh_out->indices = indices;
	mesh_out->nIndices = nIndices;
	mesh_out->indexSize = sizeof(indices[0]);
	mesh_out->nVertices = nVertices;
	mesh_out->vertexSize = vertexSize;
	mesh_out->vertexBuffers = { vertices };

	data_out.meshes = { mesh_out };

	return true;
}