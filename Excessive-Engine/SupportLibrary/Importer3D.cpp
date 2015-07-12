#include "Importer3D.h"

// Assimp
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"
#include "assimp/Scene.h"

// Util
#include <fstream>
#include <map>

bool Importer3D::LoadModelFromFile(const std::wstring& path, const rImporter3DCfg& cfg, rImporter3DData& data_out) {
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
	const aiScene* scene = importer.ReadFileFromMemory(mem, fileSize, aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_FlipWindingOrder);

	// Free memory
	free(mem);

	if (!scene) {
		return false;
	}


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
		case eImporter3DFlag::VERT_ATTR_POS:			vertexSize += sizeof(mm::vec3);  pos_attribOffset		  = offset;	offset += sizeof(mm::vec3); break;
		case eImporter3DFlag::VERT_ATTR_TEX0:			vertexSize += sizeof(mm::vec2);  tex0_attribOffset		  = offset;	offset += sizeof(mm::vec2); break;
		case eImporter3DFlag::VERT_ATTR_NORM:			vertexSize += sizeof(mm::vec3);  norm_attribOffset		  = offset;	offset += sizeof(mm::vec3); break;
		case eImporter3DFlag::VERT_ATTR_TAN:			vertexSize += sizeof(mm::vec3);  tan_attribOffset		  = offset;	offset += sizeof(mm::vec3); break;
		case eImporter3DFlag::VERT_ATTR_BITAN:			vertexSize += sizeof(mm::vec3);  bitan_attribOffset		  = offset;	offset += sizeof(mm::vec3); break;
		case eImporter3DFlag::VERT_ATTR_BONE_INDICES:	vertexSize += sizeof(u32) * 4;	 boneIndices_attribOffset = offset;	offset += sizeof(mm::vec2); break;
		case eImporter3DFlag::VERT_ATTR_BONE_WEIGHTS:	vertexSize += sizeof(float) * 4; boneWeights_attribOffset = offset;	offset += sizeof(float) * 4; break;
		}
	}

	// Copy indices, vertices from meshes
	void* vertices = new u8[vertexSize * nVertices];
	u32* indices = new u32[nIndices];

	// We collect each meshes indices and vertices into a big vertices, indices buffer, so meshes have local indexes to their local vertices
	// we have global index for containers.  GlobalIdx += localIdx
	size_t globalIndicesIdx = 0;
	size_t globalVertexIdx = 0;
	size_t vertexOffset = 0;

	// TODO: hack, will be combined to 1 mesh always, todo
	rImporter3DMesh mesh_out;
		mesh_out.materials.resize(nMeshes);

	// Each mesh
	for (size_t i = 0; i < nMeshes; i++)
	{
		aiMesh* mesh = meshes[i];

		// Get mesh material infos
		//rImporter3DMesh::rMaterial material;
		mesh_out.materials[i].faceStartIdx = globalIndicesIdx / 3;
		mesh_out.materials[i].faceEndIdx = globalIndicesIdx / 3 + mesh->mNumFaces;

		// Get Diffuse texture path
		aiString diffusePath;
		if (aiReturn_SUCCESS == scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath)) 
		{
			wchar_t unicodePath[256];
			size_t nConvertedChar;
			mbstowcs_s(&nConvertedChar, unicodePath, diffusePath.C_Str(), 256);

			std::wstring unicodePathStr = unicodePath;
			auto pos = unicodePathStr.rfind('\\');

			if (pos != std::wstring::npos)
				unicodePathStr = unicodePathStr.substr(pos + 1, unicodePathStr.size());

			std::wstring modelDirectory = path;
			auto chIdx = modelDirectory.rfind('/');
			modelDirectory = modelDirectory.substr(0, chIdx + 1);

			// Really fucking absolute path
			mesh_out.materials[i].texPathDiffuse = modelDirectory + unicodePathStr;
		}

		// Get Normal texture path
		aiString normalPath;
		if (aiReturn_SUCCESS == scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_NORMALS, 0, &normalPath)) 
		{
			wchar_t unicodePath[256];
			size_t nConvertedChar;
			mbstowcs_s(&nConvertedChar, unicodePath, normalPath.C_Str(), 256);

			std::wstring modelDirectory = path;
			auto chIdx = modelDirectory.rfind('/');
			modelDirectory = modelDirectory.substr(0, chIdx);

			mesh_out.materials[i].texPathNormal = modelDirectory + unicodePath;
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
				unsigned localVertIdx = face.mIndices[k];

				// Gather Index data
				indices[globalIndicesIdx + k] = localVertIdx + globalVertexIdx;

				// Gather position
				if (bHasPos) 
				{
					const aiVector3D& pos = mesh->mVertices[localVertIdx];
					
					// Determine vertex index
					u32 vertexIdx = localVertIdx + globalVertexIdx;

					// Then copy the data to the appropriate attrib offset in that vertex
					memcpy(((u8*)vertices) + vertexSize * vertexIdx + pos_attribOffset, &mm::vec3(pos.x, pos.y, pos.z), sizeof(mm::vec3));
				}

				// Gather normal
				if (bHasNormals)
				{
					const aiVector3D& normal = mesh->mNormals[localVertIdx];

					// Determine vertex index
					u32 vertexIdx = localVertIdx + globalVertexIdx;

					// Then copy the data to the appropriate attrib offset in that vertex
					memcpy(((u8*)vertices) + vertexSize * vertexIdx + norm_attribOffset, &mm::vec3(normal.x, normal.y, normal.z), sizeof(mm::vec3));
				}

				//if (mesh->HasTangentsAndBitangents()) {
				//	supTmpVec = &mesh->mTangents[localVertIdx];
				//	((baseVertex*)vertices)[localVertIdx + globalVertexIdx].tangent = Vec3(supTmpVec->x, supTmpVec->y, supTmpVec->z);
				//}

				// @TODO not general algorithm, wee need to handle more UV channels
				auto vecPtr = mesh->mTextureCoords[0];
				if (vecPtr)
				{
					const aiVector3D& tex0 = vecPtr[localVertIdx];

					// Determine vertex index
					u32 vertexIdx = localVertIdx + globalVertexIdx;

					// Then copy the data to the appropriate attrib offset in that vertex
					memcpy(((u8*)vertices) + vertexSize * vertexIdx + tex0_attribOffset, &mm::vec2(tex0.x, 1 - tex0.y), sizeof(mm::vec2)); // UV flip y
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
	
	mesh_out.indices = indices;
	mesh_out.nIndices = nIndices;
	mesh_out.indexSize = sizeof(indices[0]);
	mesh_out.nVertices = nVertices;
	mesh_out.vertexSize = vertexSize;
	mesh_out.vertexBuffers = { vertices };
		data_out.meshes = { mesh_out };

	return true;
}