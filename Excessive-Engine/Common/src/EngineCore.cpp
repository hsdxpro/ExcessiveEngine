#include "EngineCore.h"
#include "Factory.h"
#include "Importer3D.h"

#include "../GraphicsEngine/Raster/src/GraphicsEngineRaster.h"
#include "../GraphicsEngine/RT/src/GraphicsEngineRT.h"
#include "../PhysicsEngine/Bullet/src/PhysicsEngineBullet.h"
#include "../NetworkEngine/Boost/src/NetworkEngineBoost.h"
#include "../SoundEngine/SFML/src/SoundEngineSFML.h"

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
:graphicsEngine(0), physicsEngine(0), soundEngine(0), networkEngine(0) {
}

EngineCore::~EngineCore() {
	for (auto& a : entities)
		delete a;

	if (graphicsEngine)	graphicsEngine->release();
	if (physicsEngine)	physicsEngine->release();
	if (networkEngine)	networkEngine->release();
	if (soundEngine)	soundEngine->release();
}

graphics::IEngine* EngineCore::initGraphicsEngineRaster(const rGraphicsEngineRaster& d /*= rGraphicsEngineRaster()*/) {
	if (graphicsEngine) 
		graphicsEngine->release(); 

	return graphicsEngine = Factory::createGraphicsEngineRaster(d);
}

graphics::IEngine* EngineCore::initGraphicsEngineRT(const rGraphicsEngineRT& d /*= rGraphicsEngineRT()*/) {
	if (graphicsEngine) 
		graphicsEngine->release(); 
	
	return graphicsEngine = Factory::createGraphicsEngineRT(d);
}

physics::IEngine* EngineCore::initPhysicsEngineBullet(const rPhysicsEngineBullet& d /*= rPhysicsEngineBullet()*/) {
	if (physicsEngine)
		physicsEngine->release();

	return physicsEngine = Factory::createPhysicsEngineBullet(d);
}

network::IEngine* EngineCore::initNetworkEngine(const rNetworkEngine& d /*= rNetworkEngine()*/) {
	if (networkEngine)
		networkEngine->release();

	return networkEngine = Factory::createNetworkEngine(d);
}

sound::IEngine* EngineCore::initSoundEngine(const rSoundEngine& d /*= rSoundEngine()*/) {
	if (soundEngine)
		soundEngine->release();

	return soundEngine = Factory::createSoundEngine(d);
}

Entity* EngineCore::addEntity(graphics::IScene* gScene, const std::wstring& modelPath, float mass) {
	// Config for importing
	rImporter3DCfg cfg({ eImporter3DFlag::VERT_INTERLEAVED,
						 eImporter3DFlag::VERT_ATTR_POS,
						 eImporter3DFlag::VERT_ATTR_NORM,
						 eImporter3DFlag::VERT_ATTR_TEX0 });
	rImporter3DData modelDesc;
	Importer3D::loadFile(modelPath, cfg, modelDesc);


	// TODO: need add not set for entity, or subMeshes needed, like material -> subMaterial
	assert(modelDesc.meshes.size() <= 1);

//// --------------- GRAPHICS PART OF ENTITY ------------------------------/////

	// We will feed meshes to that graphics entity
	graphics::IEntity* gEntity = gScene->createEntity();

	// Material for entity
	graphics::IMaterial* material = graphicsEngine->createMaterial();
	gEntity->setMaterial(material);
	
	// For each mesh imported, create graphics mesh
	for (auto& importedMesh : modelDesc.meshes) {
		graphics::IMesh* graphicsMesh = graphicsEngine->createMesh();
		gEntity->setMesh(graphicsMesh);

		// Materials
		for (auto& importedMaterial : importedMesh.materials) {
			auto& subMat = material->addSubMaterial();
			subMat.base = mm::vec4(1, 1, 1, 1);

			if (importedMaterial.texPathDiffuse != L"") {
				subMat.t_diffuse = graphicsEngine->createTexture();

				// TODO:
				// turn .bmp references into .jpg (UGLY TMP)
				if (importedMaterial.texPathDiffuse.rfind(L".bmp")) {
					auto idx = importedMaterial.texPathDiffuse.rfind('.');
					auto jpgExtension = importedMaterial.texPathDiffuse.substr(0, idx + 1) + L"jpg";
					subMat.t_diffuse->load(jpgExtension.c_str());
				}
				else
					subMat.t_diffuse->load(importedMaterial.texPathDiffuse.c_str());
			}
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
			meshData.index_data		= (u32*)importedMesh.indices;
			meshData.index_num		= importedMesh.nIndices;
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

//// --------------- PHYSICS PART OF ENTITY ------------------------------/////
	physics::IEntity* pEntity = nullptr;

	//auto mesh = modelDesc.meshes[0];
	//if (mass == 0)
	//	pEntity = physicsEngine->addEntityRigidStatic((mm::vec3*)mesh.vertexBuffers[0], mesh.nVertices, mesh.indices, mesh.indexSize, mesh.nIndices);
	//else
	//	pEntity = physicsEngine->addEntityRigidDynamic((mm::vec3*)mesh.vertexBuffers[0], mesh.nVertices, mass);

	// new entity created
	Entity* e = new Entity(gEntity, pEntity);
		entities.push_back(e);
	return e;
}

void EngineCore::update(float deltaTime) {
	if (physicsEngine)	physicsEngine->update(deltaTime);
	if (graphicsEngine) graphicsEngine->update(deltaTime);
	if (soundEngine)	soundEngine->update(deltaTime);
	if (networkEngine)	networkEngine->update(deltaTime);
}
