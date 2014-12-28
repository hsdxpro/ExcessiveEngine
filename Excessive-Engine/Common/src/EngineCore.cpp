#include "EngineCore.h"
#include "Factory.h"
#include "Importer3D.h"

//////////////////////////////////////////////////
//                                              //
//           +----------+                       //
//           |   CORE   |                       //
//           +----------+                       //
//                                              //
//                 ##                           //
//                  ##                          //
//                   #                          //
//                  ___                         //
//                /  |   \                      //
//                |      |                      //
//                |------|                      //
//                |      |                      //
//                |      |                      //
//                |      |                      //
//                |      |                      //
//                |      |                      //
//                |      |                      //
//              ---     ---                     //
//            /     \ /     \                   //
//            \     / \     /                   //
//              ---     ---                     //
//                                              //
//////////////////////////////////////////////////


EngineCore::EngineCore() 
:graphicsEngine(0) {
}

EngineCore::~EngineCore() {
	for (auto& a : entities)
		delete a;
}

graphics::IGraphicsEngine* EngineCore::initGraphicsEngineRaster(const rGraphicsEngineRaster& desc) {
	if (graphicsEngine) 
		graphicsEngine->release(); 

	return graphicsEngine = Factory::createGraphicsEngineRaster(desc);
}

// Init graphics engine, if one already exists will be destroyed, then instantiate it
graphics::IGraphicsEngine* EngineCore::initGraphicsEngineRT(const rGraphicsEngineRT& desc) {
	if (graphicsEngine) 
		graphicsEngine->release(); 
	
	return Factory::createGraphicsEngineRT(desc);
}

Entity* EngineCore::createEntity(graphics::IScene* gScene, const std::wstring& modelPath)
{
	// Config for importing
	rImporter3DCfg cfg({ eImporter3DFlag::VERT_INTERLEAVED, 
						 eImporter3DFlag::VERT_ATTR_POS, 
						 eImporter3DFlag::VERT_ATTR_NORM, 
						 eImporter3DFlag::VERT_ATTR_TEX0 });
	rImporter3DData desc;
	Importer3D::loadFile(modelPath, cfg, desc);


	// TODO: need add not set for entity, or subMeshes needed, like material -> subMaterial
	assert(desc.meshes.size() <= 1);

// Creating graphics entity
	// We will feed meshes to that graphics entity
	graphics::IEntity* gEntity = gScene->createEntity();

	// Material for entity
	graphics::IMaterial* material = graphicsEngine->createMaterial();
	gEntity->setMaterial(material);
	
	// For each mesh imported, create graphics mesh
	for (auto& importedMesh : desc.meshes) {
		graphics::IMesh* graphicsMesh = graphicsEngine->createMesh();
		gEntity->setMesh(graphicsMesh);

		// Materials
		for (auto& importedMaterial : importedMesh.materials) {
			auto& subMat = material->addSubMaterial();
			subMat.base = mm::vec4(1, 1, 1, 1);
			subMat.t_diffuse = graphicsEngine->createTexture();
			//subMat.t_diffuse->load(importedMaterial.texPathDiffuse.c_str());
			subMat.t_diffuse->load((Sys::getWorkDir() + L"image.png").c_str());
		}

		// Material groups (face assignment)
		std::vector<graphics::IMesh::MaterialGroup> matIDs;
		matIDs.resize(importedMesh.materials.size());
		for (u32 i = 0; i < matIDs.size(); i++) {
			matIDs[i].beginFace = importedMesh.materials[i].faceStartIdx;
			matIDs[i].endFace = importedMesh.materials[i].faceEndIdx;
			matIDs[i].id = i;
		}

		graphics::IMesh::MeshData meshData;
			meshData.index_data		= importedMesh.indices.data();
			meshData.index_num		= importedMesh.indices.size();
			meshData.mat_ids		= matIDs.data();
			meshData.mat_ids_num	= matIDs.size();
			meshData.vertex_bytes	= importedMesh.nVertices * importedMesh.vertexSize;
			meshData.vertex_data	= importedMesh.vertexBuffers[0];

			graphics::IMesh::ElementDesc elements[] = {
				graphics::IMesh::POSITION, 3,
				graphics::IMesh::NORMAL, 3,
				graphics::IMesh::TEX0, 2,
			};
			meshData.vertex_elements = elements;
			meshData.vertex_elements_num = sizeof(elements) / sizeof(elements[0]);

		// Feed data to mesh
		graphicsMesh->update(meshData);
	}

// TODO:
// Create Physics engine
	//physicsEngine->........



	// new entity created
	Entity* e = new Entity(gEntity);
		entities.push_back(e);
	return e;
}
