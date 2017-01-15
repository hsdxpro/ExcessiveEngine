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
#include "Triangle.h"

bool Importer3D::LoadModelFromFile(const std::string& path, const rImporter3DCfg& cfg, rImporter3DData& data_out)
{
	const bool bRecalculateNormalAndTangent = false;

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
	const aiScene* scene = importer.ReadFileFromMemory( mem, fileSize, aiProcess_MakeLeftHanded );

	// Free memory
	free(mem);

	if (!scene)
		return false;

	size_t nMeshes = scene->mNumMeshes;
	aiMesh** meshes = scene->mMeshes;

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


	// Calc bounding box of each mesh, to make grid partition for their vertices, to speed up adjacency vertex search, used for smooth (tangent and normal)
	struct AABB
	{
		AABB():min(0,0,0), max(0,0,0){}

		mm::vec3 min;
		mm::vec3 max;
	};
	
	struct Face
	{
		float area;
	};

	struct Vertex
	{
		mm::vec3 pos;
		mm::vec3 normal;
		mm::vec3 tangent;
		Face face;
	};

	struct Cell
	{
		std::vector<Vertex> vertices;
	};

	struct Grid3D
	{
		void CalculateAveragedVertexAttribs(const mm::vec3& vertexPos, mm::vec3* averagedNormal_out, mm::vec3* averagedTangent_out)
		{
			Cell cell = GetCellByPos(vertexPos);

			const mm::vec3& vertexPosCur = vertexPos;

			mm::vec3 averagedNormal(0, 0, 0);
			mm::vec3 averagedTangent(0, 0, 0);
			for (auto& v : cell.vertices)
			{
				mm::vec3 d = v.pos - vertexPosCur;
				if (memcmp(&vertexPosCur, &v.pos, sizeof(mm::vec3)) == 0)
				{
					averagedNormal += v.normal * v.face.area;
					averagedTangent += v.tangent * v.face.area;
				}
			}

			if (averagedNormal_out)
				*averagedNormal_out = mm::normalize(averagedNormal);

			if (averagedTangent_out)
				*averagedTangent_out = mm::normalize(averagedTangent);
		}

		Cell& GetCellByPos(const mm::vec3& pos)
		{
			const float indexBias = 0.9999f; // If xIdx should be 0 - 9, we must multiply with that value to avoid value 10, bad indexing
			int xIdx = (int)((pos.x - aabb.min.x) / cellSize.x * indexBias);
			int yIdx = (int)((pos.y - aabb.min.y) / cellSize.y * indexBias);
			int zIdx = (int)((pos.z - aabb.min.z) / cellSize.z * indexBias);

			int cellIdx = xIdx + yIdx * nCellsOnAxis + zIdx * nCellsOnAxis * nCellsOnAxis;

			assert(cellIdx < nCellsOnAxis * nCellsOnAxis * nCellsOnAxis);
			return cells[cellIdx];
		}

		std::vector<Cell> cells;
		mm::vec3 cellSize;
		float nCellsOnAxis;
		AABB aabb;
	};

	struct Mesh
	{
		AABB aabb;
		Grid3D grid;
	};

	std::vector<Mesh> processedMeshes(nMeshes);

	if (bRecalculateNormalAndTangent)
	{
		for (size_t i = 0; i < nMeshes; i++)
		{
			aiMesh* mesh = meshes[i];

			Mesh& processedMesh = processedMeshes[i];

			mm::vec3& min = processedMesh.aabb.min;
			mm::vec3& max = processedMesh.aabb.max;


			// 1 iteration on that mesh faces, to calculate AABB for space partition bounds
			for (size_t j = 0; j < mesh->mNumFaces; j++)
			{
				aiFace& face = mesh->mFaces[j];

				aiVector3D& v0Pos = mesh->mVertices[face.mIndices[0]];
				aiVector3D& v1Pos = mesh->mVertices[face.mIndices[1]];
				aiVector3D& v2Pos = mesh->mVertices[face.mIndices[2]];

				v0Pos = meshTransformations[i] * v0Pos;
				v1Pos = meshTransformations[i] * v1Pos;
				v2Pos = meshTransformations[i] * v2Pos;

				std::swap(v0Pos.y, v0Pos.z);
				std::swap(v1Pos.y, v1Pos.z);
				std::swap(v2Pos.y, v2Pos.z);

				aiVector3D vert0Pos = mesh->mVertices[face.mIndices[0]];
				aiVector3D vert1Pos = mesh->mVertices[face.mIndices[1]];
				aiVector3D vert2Pos = mesh->mVertices[face.mIndices[2]];

				float minX = mm::min(mm::min(vert0Pos.x, vert1Pos.x), vert2Pos.x);
				float minY = mm::min(mm::min(vert0Pos.y, vert1Pos.y), vert2Pos.y);
				float minZ = mm::min(mm::min(vert0Pos.z, vert1Pos.z), vert2Pos.z);

				float maxX = mm::max(mm::max(vert0Pos.x, vert1Pos.x), vert2Pos.x);
				float maxY = mm::max(mm::max(vert0Pos.y, vert1Pos.y), vert2Pos.y);
				float maxZ = mm::max(mm::max(vert0Pos.z, vert1Pos.z), vert2Pos.z);

				if (minX < min.x)
					min.x = minX;

				if (minY < min.y)
					min.y = minY;

				if (minZ < min.z)
					min.z = minZ;

				if (maxX > max.x)
					max.x = maxX;

				if (maxY > max.y)
					max.y = maxY;

				if (maxZ > max.z)
					max.z = maxZ;
			}

			// We have the bounding box for that mesh, initialize our 3D grid
			float width = max.x - min.x;
			float height = max.y - min.y;
			float depth = max.z - min.z;

			float maxDiameter = mm::max(mm::max(width, height), depth);

			processedMesh.grid.aabb.min = min;
			processedMesh.grid.aabb.max = max;
			processedMesh.grid.nCellsOnAxis = ceil(1 + powf(mesh->mNumVertices, 1.0f / 3.0f));
			processedMesh.grid.cellSize = mm::vec3(width, height, depth) / processedMesh.grid.nCellsOnAxis;
			processedMesh.grid.cells.resize(processedMesh.grid.nCellsOnAxis * processedMesh.grid.nCellsOnAxis * processedMesh.grid.nCellsOnAxis);

			// 2. iteration on mesh faces.. to fill the grid
			for (size_t j = 0; j < mesh->mNumFaces; j++)
			{
				aiFace& face = mesh->mFaces[j];

				mm::vec3 v0Pos = (mm::vec3&)mesh->mVertices[face.mIndices[0]];
				mm::vec3 v1Pos = (mm::vec3&)mesh->mVertices[face.mIndices[1]];
				mm::vec3 v2Pos = (mm::vec3&)mesh->mVertices[face.mIndices[2]];

				mm::vec2 v0Tex0 = (mm::vec2&)(mesh->mTextureCoords[0][face.mIndices[0]]);
				mm::vec2 v1Tex0 = (mm::vec2&)(mesh->mTextureCoords[0][face.mIndices[1]]);
				mm::vec2 v2Tex0 = (mm::vec2&)(mesh->mTextureCoords[0][face.mIndices[2]]);

				mm::vec3 normal = Triangle::CalcNormal(v0Pos, v1Pos, v2Pos);
				mm::vec3 tangent = Triangle::CalcTangent(v0Pos, v1Pos, v2Pos, v0Tex0, v1Tex0, v2Tex0);

				float faceArea = Triangle::CalcArea(v0Pos, v1Pos, v2Pos);

				for (int k = 0; k < 3; ++k)
				{
					int vertexIdx = face.mIndices[k];

					Vertex v;
					v.pos = (mm::vec3&)mesh->mVertices[vertexIdx];
					v.normal = normal;
					v.tangent = tangent;
					v.face.area = faceArea;

					Cell& cell = processedMesh.grid.GetCellByPos(v.pos);
					cell.vertices.push_back(v);
				}
			}
		}
	}


	u32 nIndices = 0;
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
		Mesh& processedMesh = processedMeshes[i];

		// Get mesh material infos
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
		bool bHasNormal = mesh->HasNormals();
		bool bHasTangent = mesh->HasTangentsAndBitangents();

		// Gather Each face
		for (size_t j = 0; j < mesh->mNumFaces; globalIndicesIdx += 3, j++)
		{
			aiFace& face = mesh->mFaces[j];

			int faceIndices[3];
			faceIndices[0] = face.mIndices[0];
			faceIndices[1] = face.mIndices[1];
			faceIndices[2] = face.mIndices[2];

			const aiVector3D& vert0Pos = mesh->mVertices[faceIndices[0]];
			const aiVector3D& vert1Pos = mesh->mVertices[faceIndices[1]];
			const aiVector3D& vert2Pos = mesh->mVertices[faceIndices[2]];

			const aiVector3D& vert0UV = mesh->mTextureCoords[0][faceIndices[0]];
			const aiVector3D& vert1UV = mesh->mTextureCoords[0][faceIndices[1]];
			const aiVector3D& vert2UV = mesh->mTextureCoords[0][faceIndices[2]];

			//mm::vec3 tangent = Triangle::CalcTangent((mm::vec3&)vert0Pos, (mm::vec3&)vert1Pos, (mm::vec3&)vert2Pos, (mm::vec2&)vert0UV, (mm::vec2&)vert1UV, (mm::vec2&)vert2UV);

			// Calculate normal...
			mm::vec3 faceVertexPositions[3];
			faceVertexPositions[0] = mm::vec3(vert0Pos.x, vert0Pos.y, vert0Pos.z);
			faceVertexPositions[1] = mm::vec3(vert1Pos.x, vert1Pos.y, vert1Pos.z);
			faceVertexPositions[2] = mm::vec3(vert2Pos.x, vert2Pos.y, vert2Pos.z);

			for (int k = 0; k < 3; ++k)
			{
				u32 localVertIdx = face.mIndices[k];
				u32 vertexIdx = localVertIdx + globalVertexIdx;

				u8* vertex = ((u8*)vertices) + vertexSize * vertexIdx;

				// Gather Index data
				indices[globalIndicesIdx + k] = vertexIdx;

				// Gather position
				if (bHasPos & cfg.isContain(eImporter3DFlag::VERT_ATTR_POS))
				{
					mm::vec3 pos = faceVertexPositions[k];

					//if (!bRecalculateNormalAndTangent)
					//{
					//	std::swap(pos.y, pos.z);
					//}

					memcpy(vertex + pos_attribOffset, &pos, sizeof(mm::vec3));
				}

				mm::vec3 normal(0, 0, 0);
				mm::vec3 tangent(0, 0, 0);
				if (bRecalculateNormalAndTangent)
				{
					processedMesh.grid.CalculateAveragedVertexAttribs(faceVertexPositions[k], &normal, &tangent);
				}
				else
				{
					if (bHasNormal)
						normal = (mm::vec3&)mesh->mNormals[localVertIdx];

					if (bHasTangent)
						tangent = (mm::vec3&)mesh->mTangents[localVertIdx];
				}

				// Gather normal
				if (cfg.isContain(eImporter3DFlag::VERT_ATTR_NORM))
				{
					memcpy(vertex + norm_attribOffset, &normal, sizeof(mm::vec3));
				}

				// Gather tangents
				if (cfg.isContain(eImporter3DFlag::VERT_ATTR_TAN))
				{
					memcpy(vertex + tan_attribOffset, &tangent, sizeof(mm::vec3));
				}
				/*// Gather Tangents and Bitangents
				if (mesh->HasTangentsAndBitangents())
				{
					if (cfg.isContain(eImporter3DFlag::VERT_ATTR_TAN))
					{
						const aiVector3D& tangent = mesh->mTangents[localVertIdx];

						// Then copy the data to the appropriate attrib offset in that vertex
						memcpy(vertex + tan_attribOffset, &mm::vec3(tangent.x, tangent.y, tangent.z), sizeof(mm::vec3));
					}

					if (cfg.isContain(eImporter3DFlag::VERT_ATTR_BITAN))
					{
						const aiVector3D& bitangent = mesh->mBitangents[localVertIdx];

						// Then copy the data to the appropriate attrib offset in that vertex
						memcpy(vertex + bitan_attribOffset, &mm::vec3(bitangent.x, bitangent.y, bitangent.z), sizeof(mm::vec3));
					}
				}*/

				// @TODO not general algorithm, wee need to handle more UV channels
				aiVector3D* vecPtr = mesh->mTextureCoords[0];
				if (vecPtr != nullptr && cfg.isContain(eImporter3DFlag::VERT_ATTR_TEX0))
				{
					const aiVector3D& tex0 = vecPtr[localVertIdx];

					// Determine vertex index
					u32 vertexIdx = localVertIdx + globalVertexIdx;

					// Then copy the data to the appropriate attrib offset in that vertex
					memcpy(((u8*)vertices) + vertexSize * vertexIdx + tex0_attribOffset, &mm::vec2(tex0.x, 1 - tex0.y), sizeof(mm::vec2));
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