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
:graphicsEngine(0), physicsEngine(0), soundEngine(0), networkEngine(0) 
{
}

EngineCore::~EngineCore() 
{
	for (auto& a : entities)
		delete a;

	if (graphicsEngine)	graphicsEngine->release();
	if (physicsEngine)	physicsEngine->release();
	if (networkEngine)	networkEngine->release();
	if (soundEngine)	soundEngine->release();
}

graphics::IEngine* EngineCore::initGraphicsEngineRaster(const rGraphicsEngineRaster& d /*= rGraphicsEngineRaster()*/) 
{
	if (graphicsEngine) 
		graphicsEngine->release(); 

	return graphicsEngine = Factory::createGraphicsEngineRaster(d);
}

graphics::IEngine* EngineCore::initGraphicsEngineRT(const rGraphicsEngineRT& d /*= rGraphicsEngineRT()*/) 
{
	if (graphicsEngine) 
		graphicsEngine->release(); 
	
	return graphicsEngine = Factory::createGraphicsEngineRT(d);
}

physics::IEngine* EngineCore::initPhysicsEngineBullet(const rPhysicsEngineBullet& d /*= rPhysicsEngineBullet()*/) 
{
	if (physicsEngine)
		physicsEngine->release();

	return physicsEngine = Factory::createPhysicsEngineBullet(d);
}

network::IEngine* EngineCore::initNetworkEngine(const rNetworkEngine& d /*= rNetworkEngine()*/) 
{
	if (networkEngine)
		networkEngine->release();

	return networkEngine = Factory::createNetworkEngine(d);
}

sound::IEngine* EngineCore::initSoundEngine(const rSoundEngine& d /*= rSoundEngine()*/) 
{
	if (soundEngine)
		soundEngine->release();

	return soundEngine = Factory::createSoundEngine(d);
}

Entity* EngineCore::addEntity(graphics::IScene* gScene, const std::wstring& modelPath, float mass) 
{
	// Config for importing
	rImporter3DCfg cfg({ eImporter3DFlag::VERT_INTERLEAVED,
						 eImporter3DFlag::VERT_ATTR_POS,
						 eImporter3DFlag::VERT_ATTR_NORM,
						 eImporter3DFlag::VERT_ATTR_TEX0,
						 eImporter3DFlag::PIVOT_RECENTER });

	rImporter3DData modelDesc;
	Importer3D::loadFile(modelPath, cfg, modelDesc);


	// TODO: need add not set for entity, or subMeshes needed, like material -> subMaterial
	assert(modelDesc.meshes.size() <= 1);

	//// --------------- GRAPHICS PART OF ENTITY ------------------------------/////

	// We will feed meshes to that graphics entity
	graphics::IEntity* gEntity = gScene->addEntity();
	if (modelPath.substr(modelPath.find_last_of(L'/') + 1) == L"skybox.dae") {
		gEntity->setScale(mm::vec3(1000, 1000, 1000));
	}

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
		meshData.index_data = (u32*)importedMesh.indices;
		meshData.index_num = importedMesh.nIndices;
		meshData.mat_ids = matIDs.data();
		meshData.mat_ids_num = matIDs.size();
		meshData.vertex_bytes = importedMesh.nVertices * importedMesh.vertexSize;
		meshData.vertex_data = importedMesh.vertexBuffers[0];

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

	auto mesh = modelDesc.meshes[0];

	mm::vec3* vertices;

	if (cfg.isContain(eImporter3DFlag::VERT_INTERLEAVED)) {
		vertices = new mm::vec3[mesh.nVertices];
		for (u32 i = 0; i < mesh.nVertices; i++)
			vertices[i] = *(mm::vec3*)((u8*)mesh.vertexBuffers[0] + i * mesh.vertexSize);
	} else {
		vertices = (mm::vec3*)mesh.vertexBuffers[0];
	}

	if (mass == 0) {
		pEntity = physicsEngine->addEntityRigidStatic(vertices, mesh.nVertices, mesh.indices, mesh.indexSize, mesh.nIndices);
	}
	else
		pEntity = physicsEngine->addEntityRigidDynamic(vertices, mesh.nVertices, mass);

	delete vertices;
		vertices = nullptr; // Important

	// new entity created
	Entity* e = new Entity(gEntity, pEntity);
		entities.push_back(e);
	return e;
}

void EngineCore::update(float deltaTime, graphics::IScene* scene)
{
	/*
	// Debugging fcking bullet physics
	static graphics::IMesh* debugRenderMesh = graphicsEngine->createMesh();

	uint32_t nVertices;
	mm::vec3* nonIndexedVertices = new mm::vec3[999999];
	u32* indices = new u32[999999];
	physicsEngine->GetDebugData(nonIndexedVertices, 0, nVertices);

	for (u32 i = 0; i < nVertices; i++)
		indices[i] = i;

	graphics::IMesh::MeshData data;
		data.index_data = indices;
		data.index_num = nVertices;
		data.vertex_bytes = nVertices * sizeof(mm::vec3);
		data.vertex_data = nonIndexedVertices;
		data.vertex_elements_num = 1;
		graphics::IMesh::ElementDesc d;
			d.num_components = 3;
			d.semantic = graphics::IMesh::POSITION;
		data.vertex_elements = &d;
		data.mat_ids_num = 1;
		graphics::IMesh::MaterialGroup matGroup;
			matGroup.beginFace = 0;
			matGroup.endFace = nVertices / 3;
			matGroup.id = 0;
		data.mat_ids = &matGroup;
	debugRenderMesh->update(data);
	
	static graphics::IEntity* entity = scene->addEntity();
	entity->setMesh(debugRenderMesh);

	delete[] nonIndexedVertices;
	delete[] indices;
	*/

	// Okay physics updated, now time to send transformation to graphics
	if (physicsEngine)
		physicsEngine->update(deltaTime);

	for (auto& a : entities) 
	{
		physics::IEntity* pEntity = a->getComponentPhysics();
		graphics::IEntity* gEntity = a->getComponentGraphics();

		if (pEntity && gEntity)
		{
			gEntity->setPos(pEntity->getPos());
			gEntity->setRot(pEntity->getRot());
		}
	}

	if (graphicsEngine) 
		graphicsEngine->update(deltaTime);

	if (soundEngine)
		soundEngine->update(deltaTime);

	if (networkEngine)	
		networkEngine->update(deltaTime);
}