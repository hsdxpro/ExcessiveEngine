#include "ResourceLoader.h"

#include "../../Externals/include/assimp/Importer.hpp"
#include "../../Externals/include/assimp/Scene.h"
#include "../../Externals/include/assimp/PostProcess.h"

#include "../GraphicsEngine_Interface/interface/IMesh.h"

#include "mymath/mymath.h"

#include <fstream>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
// Mesh functions

bool ResourceLoader::loadMesh(graphics::IMesh* mesh, const wchar_t* filePath) {
	Assimp::Importer importer;
	

	std::ifstream is(filePath, std::ios::ate);
	if ( ! is.is_open()) {
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

	
	// Parsed "scene" have meshes
	size_t nMeshes = scene->mNumMeshes;
	aiMesh** meshes = scene->mMeshes;

	u32 nIndices = 0;
	//nMatGroups = nMeshes; // Assimp breaks material groups into N meshes

	size_t nVertices = 0;

	// Get indexCount, vertexCount, Gather matGroups from meshes
	//matGroups = new tMatGroup[nMatGroups];
	std::vector<u32> matIDs;
	matIDs.resize(nMeshes);
	for (size_t i = 0; i < nMeshes; i++) {
		matIDs[i] = nIndices / 3;

		// VB, IB
		nVertices += meshes[i]->mNumVertices;
		nIndices += meshes[i]->mNumFaces * 3;
	}

	// DEFINE VERTEX STRUCTURE HERE.... @TODO REMOVE IT OR I KILL MYSELF
	struct baseVertex {
		mm::vec3 pos;
		mm::vec3 normal;
		mm::vec2 tex0;
		//bool operator == (const baseVertex& v) { return pos == v.pos; }
	};

	//std::vector<cVertexFormat::Attribute> attribs;
	//
	//cVertexFormat::Attribute a;
	//// POSITION
	//a.bitsPerComponent = cVertexFormat::_32_BIT;
	//a.nComponents = 3;
	//a.semantic = cVertexFormat::POSITION;
	//a.type = cVertexFormat::FLOAT;
	//attribs.push_back(a);
	//
	//// NORMAL
	//a.bitsPerComponent = cVertexFormat::_32_BIT;
	//a.nComponents = 3;
	//a.semantic = cVertexFormat::NORMAL;
	//a.type = cVertexFormat::FLOAT;
	//attribs.push_back(a);
	//
	//// TANGENT
	//a.bitsPerComponent = cVertexFormat::_32_BIT;
	//a.nComponents = 3;
	//a.semantic = cVertexFormat::COLOR;
	//a.type = cVertexFormat::FLOAT;
	//attribs.push_back(a);
	//
	//// TEX0
	//a.bitsPerComponent = cVertexFormat::_32_BIT;
	//a.nComponents = 2;
	//a.semantic = cVertexFormat::TEXCOORD;
	//a.type = cVertexFormat::FLOAT;
	//attribs.push_back(a);
	//
	//// The vertex format !!!
	//vertexFormat.Create(attribs);


	// Copy indices, vertices from meshes
	void* vertices = new baseVertex[nVertices];
	u32* indices = new u32[nIndices];

	// Super TMP Vec3 for usage :D
	aiVector3D* supTmpVec;

	// We collect each meshes indices and vertices into a big vertices, indices buffer, so meshes have local indexes to their local vertices
	// we have global index for containers.  GlobalIdx += localIdx
	size_t globalIndicesIdx = 0;
	size_t globalVertexIdx = 0;
	size_t vertexOffset = 0;

	// Each mesh
	for (size_t i = 0; i < nMeshes; i++) {
		aiMesh* mesh = meshes[i];

		// Get mesh textures
		aiString diffusePath, normalPath;
		scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath);

		// Each face
		for (size_t j = 0; j < mesh->mNumFaces; globalIndicesIdx += 3, j++) {
			aiFace& face = mesh->mFaces[j];

			// Each vertex on face
			for (size_t k = 0; k < 3; k++) {

				unsigned localVertIdx = face.mIndices[k];

				// Index data
				indices[globalIndicesIdx + k] = localVertIdx + globalVertexIdx;

				// Vertex Data
				if (mesh->HasPositions()) {
					supTmpVec = &mesh->mVertices[localVertIdx];
					((baseVertex*)vertices)[localVertIdx + globalVertexIdx].pos = mm::vec3(supTmpVec->x, supTmpVec->y, supTmpVec->z);
				}

				if (mesh->HasNormals()) {
					supTmpVec = &mesh->mNormals[localVertIdx];
					((baseVertex*)vertices)[localVertIdx + globalVertexIdx].normal = mm::vec3(supTmpVec->x, supTmpVec->y, supTmpVec->z);
				}

				//if (mesh->HasTangentsAndBitangents()) {
				//	supTmpVec = &mesh->mTangents[localVertIdx];
				//	((baseVertex*)vertices)[localVertIdx + globalVertexIdx].tangent = Vec3(supTmpVec->x, supTmpVec->y, supTmpVec->z);
				//}

				// @TODO not general algorithm, wee need to handle more UV channels
				auto vecPtr = mesh->mTextureCoords[0];
				if (vecPtr) {
					supTmpVec = &vecPtr[localVertIdx];
					((baseVertex*)vertices)[localVertIdx + globalVertexIdx].tex0 = mm::vec2(supTmpVec->x, -supTmpVec->y); // UV flip y
				}
			}
		}
		globalVertexIdx += mesh->mNumVertices;
	}

	graphics::IMesh::MeshData data;
		data.index_data = indices;
		data.index_num = nIndices;
		data.vertex_bytes = nVertices * sizeof(baseVertex);
		data.vertex_data = vertices;
		graphics::IMesh::ElementDesc elements[] = {
			graphics::IMesh::POSITION, 3,
			graphics::IMesh::NORMAL, 3,
			graphics::IMesh::TEX0, 2,
		};
		data.vertex_elements = elements;
		data.vertex_elements_num = sizeof(elements) / sizeof(elements[0]);
		data.mat_ids = matIDs.data();
		data.mat_ids_num = matIDs.size();
	return mesh->update(data);
}

bool ResourceLoader::genMesh_Teapot(graphics::IMesh* mesh, float radius, int tesselation) {
	return false;
}

bool ResourceLoader::getMesh_Box(graphics::IMesh* mesh, float length, float width, float heigh) {
	return false;
}

bool ResourceLoader::genMesh_Sphere(graphics::IMesh*, float radius, int tesselation) {
	return false;
}

bool ResourceLoader::genMesh_Cylinder(graphics::IMesh* mesh, float radius, float height, int num_sides) {
	return false;
}


////////////////////////////////////////////////////////////////////////////////
// Material functions

bool ResourceLoader::loadMaterial(graphics::IMaterial* material, const wchar_t* filePath) {
	return false;
}


////////////////////////////////////////////////////////////////////////////////
// Texture functions

bool ResourceLoader::loadTexture(graphics::ITexture* texture, const wchar_t* filePath) {
	return false;
}
bool ResourceLoader::genTexture_Checker(graphics::ITexture* texture, int div_x, int div_y, u32 color1, u32 color2) {
	return false;
}