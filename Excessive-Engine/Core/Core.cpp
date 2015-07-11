#include "Core.h"
#include "PlatformLibrary\Sys.h"
#include "SupportLibrary\VisualCpuProfiler.h"
#include "Factory.h"

Core gCore;

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

Core::Core() 
:graphicsEngine(0), physicsEngine(0), soundEngine(0), networkEngine(0)
{

}

Core::~Core()
{
	for (auto& a : scripts)
		delete a;

	for (auto& a : worldComponents)
		delete a;

	for (auto& a : actorScripts)
		delete a;

	for (auto& a : actors)
		delete a;

	for (auto& a : importedModels)
		delete a.second;

	if (graphicsEngine)	graphicsEngine->Release();
	if (physicsEngine)	physicsEngine->Release();
	if (networkEngine)	networkEngine->Release();
	if (soundEngine)	soundEngine->Release();
}

graphics::IEngine* Core::InitGraphicsEngineRaster(const rGraphicsEngineRaster& d /*= rGraphicsEngineRaster()*/) 
{
	if (graphicsEngine)
		graphicsEngine->Release();

	graphicsEngine = Factory::CreateGraphicsEngineRaster(d);

	// Load error diffuse texture, that we place on materials which fails load their own texture by path
	texError = graphicsEngine->CreateTexture();
	
	bool bSuccess = texError->Load(Sys::GetWorkDir() + L"Assets/error.jpg");
	assert(bSuccess);

	// Default scene and layer for GraphicsEngine
	defaultGraphicsScene = graphicsEngine->CreateScene();
	graphics::IEngine::Layer layer;
	layer.scene = defaultGraphicsScene;
	graphicsEngine->AddLayer(layer);

	return graphicsEngine;
}

graphics::IEngine* Core::InitGraphicsEngineRT(const rGraphicsEngineRT& d /*= rGraphicsEngineRT()*/) 
{
	if (graphicsEngine) 
		graphicsEngine->Release(); 
	
	return graphicsEngine = Factory::CreateGraphicsEngineRT(d);
}

physics::IEngine* Core::InitPhysicsEngineBullet(const rPhysicsEngineBullet& d /*= rPhysicsEngineBullet()*/) 
{
	if (physicsEngine)
		physicsEngine->Release();

	return physicsEngine = Factory::CreatePhysicsEngineBullet(d);
}

network::IEngine* Core::InitNetworkEngine(const rNetworkEngine& d /*= rNetworkEngine()*/) 
{
	if (networkEngine)
		networkEngine->Release();

	return networkEngine = Factory::CreateNetworkEngine(d);
}

sound::IEngine* Core::InitSoundEngine(const rSoundEngine& d /*= rSoundEngine()*/) 
{
	if (soundEngine)
		soundEngine->Release();

	return soundEngine = Factory::CreateSoundEngine(d);
}

Thing* Core::SpawnThing(ActorScript* s)
{
	// New thing
	Thing* thing = new Thing();

		// Behavior from script
		ActorBehavior* behav = new ActorBehavior();
		behav->AddActorScript(s);

	thing->AddBehavior(behav);
	thing->SetActor(s->GetActor());

	return thing;
}

Actor* Core::AddActor()
{
	Actor* p = new Actor();
		actors.push_back(p);
	return p;
}

GraphicsComponent* Core::SpawnCompGraphicsFromFile(const std::wstring& modelFilePath)
{
	// Check if model already loaded somehow
	rImporter3DData* modelDesc;
	auto it = importedModels.find(modelFilePath);
	if (it != importedModels.end())
	{
		modelDesc = it->second;
	}
	else // Not found, import it...
	{
		// Config for importing
		rImporter3DCfg cfg({ eImporter3DFlag::VERT_BUFF_INTERLEAVED,
			eImporter3DFlag::VERT_ATTR_POS,
			eImporter3DFlag::VERT_ATTR_NORM,
			eImporter3DFlag::VERT_ATTR_TEX0,
			eImporter3DFlag::PIVOT_RECENTER });

		modelDesc = new rImporter3DData();
		Importer3D::LoadModelFromFile(modelFilePath, cfg, *modelDesc);

		importedModels[modelFilePath] = modelDesc;
	}

	graphics::IEntity* graphicsEntity;

	// We will feed meshes to that graphics entity
	graphicsEntity = defaultGraphicsScene->AddEntity();

	// Material for entity
	graphics::IMaterial* material = graphicsEngine->CreateMaterial();
	graphicsEntity->SetMaterial(material);

	// For each mesh imported, Create graphics mesh
	for (auto& importedMesh : modelDesc->meshes)
	{
		graphics::IMesh* graphicsMesh = graphicsEngine->CreateMesh();
		graphicsEntity->SetMesh(graphicsMesh);

		// Materials
		for (auto& importedMaterial : importedMesh.materials)
		{
			auto& subMat = material->AddSubMaterial();
			subMat.base = mm::vec4(1, 1, 1, 1);
			subMat.t_diffuse = texError; // Default is error texture !!

			if (importedMaterial.texPathDiffuse != L"")
			{
				// TODO:
				// turn .bmp references into .jpg (UGLY TMP)
				std::wstring finalPath;
				if (importedMaterial.texPathDiffuse.rfind(L".bmp"))
				{
					auto idx = importedMaterial.texPathDiffuse.rfind('.');
					auto jpgExtension = importedMaterial.texPathDiffuse.substr(0, idx + 1) + L"jpg";
					finalPath = jpgExtension;
				}
				else
				{
					finalPath = importedMaterial.texPathDiffuse;
				}

				auto texDiffuse = graphicsEngine->CreateTexture();
				if (texDiffuse->Load(finalPath.c_str()))
					subMat.t_diffuse = texDiffuse;
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
		graphicsMesh->Update(meshData);
	}

	auto c = new GraphicsComponent(graphicsEntity);
	worldComponents.push_back(c);
	return c;
}

RigidBodyComponent* Core::SpawnCompRigidBodyFromFile(const std::wstring& modelFilePath, float mass)
{
	// Check if model already loaded somehow
	rImporter3DData* modelDesc;
	auto it = importedModels.find(modelFilePath);
	if (it != importedModels.end())
	{
		modelDesc = it->second;
	}
	else // Not found, import it...
	{
		// Config for importing
		rImporter3DCfg cfg({ eImporter3DFlag::VERT_BUFF_INTERLEAVED,
			eImporter3DFlag::VERT_ATTR_POS,
			eImporter3DFlag::VERT_ATTR_NORM,
			eImporter3DFlag::VERT_ATTR_TEX0,
			eImporter3DFlag::PIVOT_RECENTER });

		modelDesc = new rImporter3DData();
		Importer3D::LoadModelFromFile(modelFilePath, cfg, *modelDesc);

		importedModels[modelFilePath] = modelDesc;
	}

	physics::IRigidEntity* rigidEntity = nullptr;

	auto mesh = modelDesc->meshes[0];

	mm::vec3* vertices;
	// Little hekk, we know it's INTERLEAVED, cuz  SpawnCompRigidBodyFromFile and SpawnCompGraphicsFromFile implementation
	//if (cfg.isContain(eImporter3DFlag::VERT_BUFF_INTERLEAVED)) // Interleaved buffer? Okay gather positions from vertices stepping with vertex stride
	{
		vertices = new mm::vec3[mesh.nVertices];
		for (u32 i = 0; i < mesh.nVertices; i++)
			vertices[i] = *(mm::vec3*)((u8*)mesh.vertexBuffers[0] + i * mesh.vertexSize);
	}
	//else
	//{
	//	vertices = (mm::vec3*)mesh.vertexBuffers[0]; // Non interleaved buffers, cool 0.th buffer will be position
	//}

	if (mass == 0)
		rigidEntity = physicsEngine->AddEntityRigidStatic(vertices, mesh.nVertices, mesh.indices, mesh.indexSize, mesh.nIndices);
	else
		rigidEntity = physicsEngine->AddEntityRigidDynamic(vertices, mesh.nVertices, mass);

	delete vertices;
	vertices = nullptr; // Important

	auto c = new RigidBodyComponent(rigidEntity);
	worldComponents.push_back(c);
	return c;
}

RigidBodyComponent* Core::SpawnCompRigidBodyCapsule(float height, float radius, float mass /* = 0*/)
{
	auto capsuleEntity = physicsEngine->AddEntityRigidCapsule(height, radius, mass);

	auto c = new RigidBodyComponent(capsuleEntity);
	worldComponents.push_back(c);
	return c;
}

CameraComponent* Core::SpawnCompCamera()
{
	auto c = new CameraComponent(graphicsEngine->CreateCam());
	worldComponents.push_back(c);
	return c;
}

void Core::SetCam(CameraComponent* c)
{
	defaultGraphicsScene->SetCamera(c->GetCam());
}

void Core::Update(float deltaTime)
{
	/*
	// Debugging fcking bullet physics
	static graphics::IMesh* debugRenderMesh = graphicsEngine->CreateMesh();

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
	debugRenderMesh->Update(data);
	
	static graphics::IEntity* entity = defaultGraphicsScene->AddEntity();
	entity->SetMesh(debugRenderMesh);

	delete[] nonIndexedVertices;
	delete[] indices;
	*/

	if (physicsEngine)
	{
		PROFILE_SCOPE("PhysicsEngine");
		physicsEngine->Update(deltaTime);
	}

	// Okay physics simulation done, move high level physics component's attached WorldComponents
	// TODO, ez kibaszottul nem mukodik jol es redundans tree call - t csinál WorldComponent - eken belül

	// TODO
	//PROFILE_SECTION_BEGIN("EngineCore Component Update After Physics");
	{
		PROFILE_SCOPE("EngineCore Component Update After Physics");
		for (auto a : worldComponents)
			a->UpdateAfterPhysicsSimulate();
	}
	//PROFILE_SECTION_END();
	

	if (graphicsEngine)
	{
		PROFILE_SCOPE("GraphicsEngine");

#ifdef PROFILE_ENGINE
		graphicsEngine->GetGapi()->ResetStatesToDefault(); // Jesus the profiler also uses OpenGL temporarily, and mess up the binds etc...
#endif
		graphicsEngine->Update(deltaTime);
	}

	if (soundEngine)
	{
		PROFILE_SCOPE("SoundEngine");
		soundEngine->Update(deltaTime);
	}
		

	if (networkEngine)
	{
		PROFILE_SCOPE("NetworkEngine");
		networkEngine->Update(deltaTime);
	}

#ifdef PROFILE_ENGINE
	VisualCpuProfiler::UpdateAndPresent();
#endif

	// Present opengl window
	graphicsEngine->GetTargetWindow()->Present();
}

IWindow* Core::GetTargetWindow()
{
	return graphicsEngine->GetTargetWindow();
}

graphics::IEngine* Core::GetGraphicsEngine()
{ 
	return graphicsEngine; 
}

physics::IEngine* Core::GetPhysicsEngine() 
{ 
	return physicsEngine; 
}

network::IEngine* Core::GetNetworkEngine() 
{ 
	return networkEngine; 
}

sound::IEngine*	Core::GetSoundEngine()
{ 
	return soundEngine; 
}