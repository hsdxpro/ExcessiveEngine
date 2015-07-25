#include "Core.h"
#include "PlatformLibrary\Sys.h"
#include "SupportLibrary\VisualCpuProfiler.h"
#include "Factory.h"

Core* gCore = nullptr;

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

	for (auto& a : entityScripts)
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

IGraphicsEngine* Core::InitGraphicsEngineRaster(const rGraphicsEngineRaster& d /*= rGraphicsEngineRaster()*/) 
{
	if (graphicsEngine)
		graphicsEngine->Release();

	// Make data from description to startup graphics engine
	rGraphicsEngineRasterData graphicsEngineData;
		graphicsEngineData.renderRegion = d.renderRegion;
		graphicsEngineData.targetWindow = d.targetWindow;
		switch(d.gapiType)
		{
			case eGapiType::OPENGL_4_5:
			{
				graphicsEngineData.gapi = Factory::CreateGapiGL();
				break;
			}

			default:
				assert(0);
		}
	graphicsEngine = Factory::CreateGraphicsEngineRaster(graphicsEngineData);

	// Load error diffuse texture, that we place on materials which fails load their own texture by path
	texError = graphicsEngine->CreateTexture();
	
	bool bSuccess = texError->Load(Sys::GetWorkDir() + L"Assets/error.jpg");
	assert(bSuccess);

	// Default scene and layer for GraphicsEngine
	defaultGraphicsScene = graphicsEngine->CreateScene();
	IGraphicsEngine::Layer layer;
	layer.scene = defaultGraphicsScene;
	graphicsEngine->AddLayer(layer);

	return graphicsEngine;
}

IGraphicsEngine* Core::InitGraphicsEngineRT(const rGraphicsEngineRT& d /*= rGraphicsEngineRT()*/) 
{
	if (graphicsEngine)
		graphicsEngine->Release();
	
	return graphicsEngine = Factory::CreateGraphicsEngineRT(d);
}

IPhysicsEngine* Core::InitPhysicsEngineBullet(const rPhysicsEngineBullet& d /*= rPhysicsEngineBullet()*/) 
{
	if (physicsEngine)
		physicsEngine->Release();

	return physicsEngine = Factory::CreatePhysicsEngineBullet(d);
}

INetworkEngine* Core::InitNetworkEngine(const rNetworkEngine& d /*= rNetworkEngine()*/) 
{
	if (networkEngine)
		networkEngine->Release();

	return networkEngine = Factory::CreateNetworkEngine(d);
}

ISoundEngine* Core::InitSoundEngineSFML(const rSoundEngine& d /*= rSoundEngine()*/) 
{
	if (soundEngine)
		soundEngine->Release();

	soundEngine = Factory::CreateSoundEngine(d);

	defaultSoundScene = soundEngine->CreateScene();

	return soundEngine;
}

bool Core::PlaySoundMono(const std::wstring& filePath, float volumeNormedPercent /*= 1*/, bool bLoop /*= false*/)
{
	sound::IEmitter* soundEmitter;
	sound::ISoundData* soundData;

	auto it = importedSounds.find(filePath);
	if(it != importedSounds.end())
	{
		soundData = it->second.soundData;
		soundEmitter = it->second.soundEmitter;
	}
	else
	{
		soundData = soundEngine->CreateSoundData();
		if (!soundData->Load((Sys::GetWorkDir() + filePath).c_str(), sound::StoreMode::BUFFERED))
		{
			soundData->Release();
			return false;
		}

		soundEmitter = defaultSoundScene->AddEmitter();
		soundEmitter->SetSoundData(soundData);
		soundEmitter->SetLooped(bLoop);

		rMonoSound d;
			d.soundData = soundData;
			d.soundEmitter = soundEmitter;
		importedSounds[filePath] = d;
	}

	soundEmitter->SetVolume(volumeNormedPercent);
	soundEmitter->Start();

	return true;
}

Actor* Core::SpawnActor(EntityScript* s)
{
	// New actor
	Actor* actor = new Actor();

		// Behavior from script
		Behavior* behav = new Behavior();
		behav->AddScript(s);

		actor->AddBehavior(behav);
		actor->SetEntity(s->GetEntity());

		actors.push_back(actor);
	return actor;
}

void Core::DestroyActor(Actor* a)
{
	actorsToDestroy.push_back(a);
}

void Core::DestroyComp(WorldComponent* c)
{
	if (dynamic_cast<GraphicsComponent*>(c))
		defaultGraphicsScene->Remove(((GraphicsComponent*)c)->GetEntity());
	else if (dynamic_cast<RigidBodyComponent*>(c))
		physicsEngine->RemoveEntity(((RigidBodyComponent*)c)->GetEntity());
	else
		assert(0);

	auto it = std::find(worldComponents.begin(), worldComponents.end(), c);
	if (it != worldComponents.end())
	{
		delete *it;
		worldComponents.erase(it);
	}
}

Actor* Core::SpawnActor()
{
	Actor* actor = new Actor();
		actors.push_back(actor);
		return actor;
}

Actor* Core::SpawnActor_MeshFromFile(const std::wstring& modelFilePath)
{
	auto actor = SpawnActor();
		auto entity = AddEntity();
		entity->SetRootComp(SpawnComp_MeshFromFile(modelFilePath));

	actor->SetEntity(entity);
	return actor;
}

Actor* Core::SpawnActor_RigidBodyFromFile(const std::wstring& modelFilePath, float mass)
{
	auto actor = SpawnActor();
		auto entity = AddEntity();
		entity->SetRootComp(SpawnComp_RigidBodyFromFile(modelFilePath, mass));

	actor->SetEntity(entity);
	return actor;
}

Actor* Core::SpawnActor_RigidBodyCapsule(float height, float radius, float mass /*= 0*/)
{
	auto actor = SpawnActor();
		auto entity = AddEntity();
		entity->SetRootComp(SpawnComp_RigidBodyCapsule(height, radius, mass));

	actor->SetEntity(entity);
	return actor;
}

Actor* Core::SpawnActor_Camera()
{
	auto actor = SpawnActor();
		auto entity = AddEntity();
		entity->SetRootComp(SpawnComp_Camera());

	actor->SetEntity(entity);
	return actor;
}

void Core::AddTask(const std::function<void()>& callb, float timeToProceed)
{
	rTask task;
		task.callb = callb;
		task.timeLeft = timeToProceed;
	tasks.push_back(task); // TODO slow
}

Entity* Core::AddEntity()
{
	Entity* p = new Entity();
		entities.push_back(p);
	return p;
}

GraphicsComponent* Core::SpawnComp_MeshFromFile(const std::wstring& modelFilePath)
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
		Importer3D::LoadModelFromFile(Sys::GetWorkDir() + modelFilePath, cfg, *modelDesc);

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

RigidBodyComponent* Core::SpawnComp_RigidBodyFromFile(const std::wstring& modelFilePath, float mass)
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
		Importer3D::LoadModelFromFile(Sys::GetWorkDir() + modelFilePath, cfg, *modelDesc);

		importedModels[modelFilePath] = modelDesc;
	}

	physics::IRigidBodyEntity* rigidEntity = nullptr;

	auto mesh = modelDesc->meshes[0];

	mm::vec3* vertices;
	// Little hekk, we know it's INTERLEAVED, cuz  SpawnCompRigidBodyFromFile and SpawnCompMeshFromFile implementation
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

RigidBodyComponent* Core::SpawnComp_RigidBodyCapsule(float height, float radius, float mass /* = 0*/)
{
	auto capsuleEntity = physicsEngine->AddEntityRigidCapsule(height, radius, mass);

	auto c = new RigidBodyComponent(capsuleEntity);
	worldComponents.push_back(c);
	return c;
}

CameraComponent* Core::SpawnComp_Camera()
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

	// Destroy actors which on destroy queue
	for (auto& a : actorsToDestroy)
	{
		// Remove from list
		auto it = std::find(actors.begin(), actors.end(), a);
		if (it != actors.end())
			actors.erase(it);

		for (auto& comp : a->GetComponents())
			DestroyComp(comp);

		delete a;
	}
	actorsToDestroy.clear();

	// Update physics
	if (physicsEngine)
	{
		PROFILE_SCOPE("Physics");
		physicsEngine->Update(deltaTime);
	}

	// Update components after physics simulation
	{
		PROFILE_SCOPE("Component Update After Physics");
		for (auto a : worldComponents)
			a->UpdateAfterPhysicsSimulate();
	}
	
	// Update graphics
	if (graphicsEngine)
	{
		PROFILE_SCOPE("Graphics");

#ifdef PROFILE_ENGINE
		graphicsEngine->GetGapi()->ResetStatesToDefault(); // Jesus the profiler also uses OpenGL temporarily, and mess up the binds etc...
#endif
		graphicsEngine->Update(deltaTime);
	}

	// Update sound
	if (soundEngine)
	{
		PROFILE_SCOPE("Sound");
		soundEngine->Update(deltaTime);
	}
		
	// Update network
	if (networkEngine)
	{
		PROFILE_SCOPE("Network");
		networkEngine->Update(deltaTime);
	}

	// Update actors
	{
		PROFILE_SCOPE("Actors Update");
		for (auto& a : actors)
			a->Update(deltaTime);
	}

	// Update game logic
	{
		PROFILE_SCOPE("Scripts");
		for (auto& s : scripts)
			s->Update(deltaTime);
	}

	{
		PROFILE_SCOPE("Entity Scripts");
		for (auto& s : entityScripts)
			s->Update(deltaTime);
	}

	// TODO optimize
	// Process Tasks if time passed, and remove after dispatch
	{
		PROFILE_SCOPE("AddTask Dispatch");
		std::vector<size_t> indicesToDelete;
		size_t oldSize = tasks.size(); // Cuz you can AddTask in AddTask ^^, tasks.size() can change in loop body
		for (size_t i = 0; i < oldSize; i++)
		{
			tasks[i].timeLeft -= deltaTime;

			if (tasks[i].timeLeft <= 0)
			{
				tasks[i].callb();
				indicesToDelete.push_back(i);
			}
		}

		// Remove dispatched taks, reverse iteration for: indices don't slip away
		for (i32 i = indicesToDelete.size(); --i >= 0;)
			tasks.erase(tasks.begin() + indicesToDelete[i]);
	}


	// Profiling engine
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

IGraphicsEngine* Core::GetGraphicsEngine()
{ 
	return graphicsEngine; 
}

IPhysicsEngine* Core::GetPhysicsEngine() 
{ 
	return physicsEngine; 
}

INetworkEngine* Core::GetNetworkEngine() 
{ 
	return networkEngine; 
}

ISoundEngine*	Core::GetSoundEngine()
{ 
	return soundEngine; 
}