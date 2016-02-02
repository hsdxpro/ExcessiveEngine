#include "EngineCore.h"
#include "PhysicsEngine\PhysicsCommon.h"
#include "PlatformLibrary\Sys.h"
#include "PlatformLibrary\File.h"
#include "SupportLibrary\VisualCpuProfiler.h"
#include "Script.h"
#include "GraphicsApi_OpenGL/GapiGL.h"
#include "GraphicsApi_DX11/GapiDX11.h"

#include <array>

EngineCore Core;

EngineCore::EngineCore()
:graphicsEngine(0), physicsEngine(0), soundEngine(0), networkEngine(0)
{
	prevFrameActorCollideList.reserve(1000);
	curFrameActorCollideList.reserve(1000);
}

EngineCore::~EngineCore()
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

	for (auto& a : importedTextures)
		a.second->Release();

	if (graphicsEngine)	graphicsEngine->Release();
	if (physicsEngine)	physicsEngine->Release();
	if (networkEngine)	networkEngine->Release();
	if (soundEngine)	soundEngine->Release();
}

IGraphicsEngine* EngineCore::InitRasterGraphicsEngine(const rRasterGraphicsEngine& d /*= rRasterGraphicsEngine()*/)
{
	if (graphicsEngine)
		graphicsEngine->Release();

	graphicsEngine = new RasterGraphicsEngine(d);

	// Load error diffuse texture, that we place on materials which fails load their own texture by path
	texError = graphicsEngine->CreateTexture();
	
	bool bSuccess = texError->Load(GetAssetsPath() + "error.jpg");
	assert(bSuccess);

	// Default scene and layer for GraphicsEngine
	defaultGraphicsScene = graphicsEngine->CreateScene();
	IGraphicsEngine::Layer layer;
	layer.scene = defaultGraphicsScene;
	graphicsEngine->AddLayer(layer);

	return graphicsEngine;
}

IGraphicsEngine* EngineCore::InitGraphicsEngineRT(const rGraphicsEngineRT& d /*= rGraphicsEngineRT()*/)
{
	if (graphicsEngine)
		graphicsEngine->Release();
	
	graphicsEngine = new GraphicsEngineRT(d);

	// Default scene and layer for GraphicsEngine
	defaultGraphicsScene = graphicsEngine->CreateScene();
	IGraphicsEngine::Layer layer;
	layer.scene = defaultGraphicsScene;
	graphicsEngine->AddLayer(layer);

	return graphicsEngine;
}

IPhysicsEngine* EngineCore::InitPhysicsEngineBullet(const rPhysicsEngineBullet& d /*= rPhysicsEngineBullet()*/)
{
	if (physicsEngine)
		physicsEngine->Release();

	return physicsEngine = new PhysicsEngineBullet(d);
}

INetworkEngine* EngineCore::InitNetworkEngineRakNet(const rNetworkEngine& d /*= rNetworkEngine()*/)
{
	if (networkEngine)
		networkEngine->Release();

	return networkEngine = new NetworkEngineRakNet(d);
}

ISoundEngine* EngineCore::InitSoundEngineSFML(const rSoundEngine& d /*= rSoundEngine()*/)
{
	if (soundEngine)
		soundEngine->Release();

	soundEngine = new SoundEngineSFML(d);

	defaultSoundScene = soundEngine->CreateScene();

	return soundEngine;
}

bool EngineCore::TraceClosestPoint_Physics(const mm::vec3& from, const mm::vec3& to, rPhysicsTraceResult& traceResult_out, const rPhysicsTraceParams& params /*= rPhysicsTraceParams()*/)
{
	physics::rTraceResult result;
	if (physicsEngine->TraceClosestPoint(from, to, result, params))
	{
		traceResult_out.pos = result.pos;
		traceResult_out.normal = result.normal;

		RigidBodyEntity* entity = (RigidBodyEntity*)result.userPointer;

		if (entity)
			traceResult_out.actor = (Actor*)entity->GetUserPointer();
		else
			traceResult_out.actor = nullptr;

		return true;
	}
	return false;
}

sound::IEmitter* EngineCore::CreateSoundMono(const std::string& filePath, float volumeNormedPercent /*= 1*/, bool bLoop /*= false*/)
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
		if (!soundData->Load((GetAssetsPath() + filePath).c_str(), sound::StoreMode::BUFFERED))
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
	return soundEmitter;
}

void EngineCore::Destroy(Actor* a)
{
	if (!a->IsKilled())
	{
		a->Kill();
		actorsToDestroy.push_back(a);
	}
}

void EngineCore::Destroy(WorldComponent* c)
{
	if (dynamic_cast<MeshComponent*>(c))
		defaultGraphicsScene->Remove(((MeshComponent*)c)->GetEntity());
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

Actor* EngineCore::SpawnActor(WorldComponent* rootComp /*= new Transform3DComponent()*/)
{
	Actor* actor = new Actor(rootComp);
	actors.push_back(actor);
	return actor;
}

Actor* EngineCore::SpawnActor_MeshFromFile(const std::string& modelFilePath)
{
	Actor* actor = SpawnActor(SpawnComp_MeshFromFile(modelFilePath));
	return actor;
}

Actor* EngineCore::SpawnActor_RigidBodyFromFile(const std::string& modelFilePath, float mass)
{
	RigidBodyComponent* rigidComp = SpawnComp_RigidBodyFromFile(modelFilePath, mass);
		Actor* actor = SpawnActor(rigidComp);
	rigidComp->SetUserPointer(actor);
	
	return actor;
}

Actor* EngineCore::SpawnActor_RigidBodyCapsule(float height, float radius, float mass /*= 0*/)
{
	RigidBodyComponent* rigidComp = SpawnComp_RigidBodyCapsule(height, radius, mass);
		Actor* actor = SpawnActor(rigidComp);
	rigidComp->SetUserPointer(actor);

	return actor;
}

Actor* EngineCore::SpawnActor_Camera()
{
	return SpawnActor(SpawnComp_Camera());
}

void EngineCore::AddTask(const std::function<void()>& callb, float timeToProceed)
{
	rTask task;
		task.callb = callb;
		task.timeLeft = timeToProceed;
	tasks.push_back(task); // TODO slow
}

MeshComponent* EngineCore::SpawnComp_MeshFromFile(const std::string& modelFilePath)
{
	// Check if model already loaded somehow
	// Check if model already loaded somehow
	rImporter3DData* modelDesc;
	auto it = importedModels.find(modelFilePath);
	if (it != importedModels.end())
	{
		modelDesc = it->second;
	}
	else // Not loaded, check bin format first
	{
		std::string binPath = GetAssetsPath() + modelFilePath.substr(0, modelFilePath.rfind('.')) + ".exm"; // Excessive Mesh

		if (File::IsExists(binPath))
		{
			modelDesc = new rImporter3DData();
			modelDesc->DeSerialize(binPath);
		}
		else
		{
			// Config for importing
			rImporter3DCfg cfg({ eImporter3DFlag::VERT_BUFF_INTERLEAVED,
				eImporter3DFlag::VERT_ATTR_POS,
				eImporter3DFlag::VERT_ATTR_NORM,
				eImporter3DFlag::VERT_ATTR_TEX0,
				eImporter3DFlag::PIVOT_RECENTER });

			modelDesc = new rImporter3DData();
			Importer3D::LoadModelFromFile(GetAssetsPath() + modelFilePath, cfg, *modelDesc);

			modelDesc->Serialize(binPath);
		}

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
		for (auto& importedMaterial : importedMesh->materials)
		{
			auto& subMat = material->AddSubMaterial();
			subMat.base = mm::vec4(1, 1, 1, 1);
			subMat.t_diffuse = texError; // Default is error texture !!

			if (importedMaterial.relTexPathDiffuse != "")
			{
				// TODO:
				// turn .bmp references into .jpg (UGLY TMP)
				// Todo, ".bmp", ".BMP"...
				std::string relPath;
				if (importedMaterial.relTexPathDiffuse.rfind(".bmp") != std::string::npos || importedMaterial.relTexPathDiffuse.rfind(".BMP") != std::string::npos)
				{
					auto idx = importedMaterial.relTexPathDiffuse.rfind('.');
					auto jpgExtension = importedMaterial.relTexPathDiffuse.substr(0, idx + 1) + "jpg";
					relPath = jpgExtension;
				}
				else
				{
					relPath = importedMaterial.relTexPathDiffuse;
				}

				graphics::ITexture* texDiffuse;
				auto it = importedTextures.find(relPath);
				if (it != importedTextures.end())
				{
					texDiffuse = it->second;
				}
				else
				{
					texDiffuse = graphicsEngine->CreateTexture();
					if (texDiffuse->Load(GetAssetsPath() + relPath))
						importedTextures[relPath] = texDiffuse;
					else
						texDiffuse = texError;
				}
				subMat.t_diffuse = texDiffuse;
			}
		}

		// Material groups (face assignment)
		std::vector<graphics::IMesh::MaterialGroup> matIDs;
		matIDs.resize(importedMesh->materials.size());
		for (u32 i = 0; i < matIDs.size(); i++) 
		{
			matIDs[i].beginFace = importedMesh->materials[i].faceStartIdx;
			matIDs[i].endFace = importedMesh->materials[i].faceEndIdx;
			matIDs[i].id = i;
		}

		graphics::IMesh::MeshData meshData;
		meshData.index_data = (u32*)importedMesh->indices;
		meshData.index_num = importedMesh->nIndices;
		meshData.mat_ids = matIDs.data();
		meshData.mat_ids_num = (u32)matIDs.size();
		meshData.vertex_bytes = importedMesh->nVertices * importedMesh->vertexSize;
		meshData.vertex_data = importedMesh->vertexBuffers[0];

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

	auto c = new MeshComponent(graphicsEntity);
	worldComponents.push_back(c);
	return c;
}

RigidBodyComponent* EngineCore::SpawnComp_RigidBodyFromFile(const std::string& modelFilePath, float mass)
{
	// Check if model already loaded somehow
	rImporter3DData* modelDesc;
	auto it = importedModels.find(modelFilePath);
	if (it != importedModels.end())
	{
		modelDesc = it->second;
	}
	else // Not loaded, check bin format first
	{
		std::string binPath = GetAssetsPath() + modelFilePath.substr(0, modelFilePath.rfind('.')) + ".exm"; // Excessive Mesh

		if (File::IsExists(binPath))
		{
			modelDesc = new rImporter3DData();
			modelDesc->DeSerialize(binPath);
		}
		else
		{
			// Config for importing
			rImporter3DCfg cfg({ eImporter3DFlag::VERT_BUFF_INTERLEAVED,
				eImporter3DFlag::VERT_ATTR_POS,
				eImporter3DFlag::VERT_ATTR_NORM,
				eImporter3DFlag::VERT_ATTR_TEX0,
				eImporter3DFlag::PIVOT_RECENTER });

			modelDesc = new rImporter3DData();
			Importer3D::LoadModelFromFile(GetAssetsPath() + modelFilePath, cfg, *modelDesc);

			modelDesc->Serialize(binPath);
		}

		importedModels[modelFilePath] = modelDesc;
	}

	physics::IRigidBodyEntity* rigidEntity = nullptr;

	auto mesh = modelDesc->meshes[0];

	mm::vec3* vertices;
	// Little hekk, we know it's INTERLEAVED, cuz  SpawnCompRigidBodyFromFile and SpawnCompMeshFromFile implementation
	//if (cfg.isContain(eImporter3DFlag::VERT_BUFF_INTERLEAVED)) // Interleaved buffer? Okay gather positions from vertices stepping with vertex stride
	{
		vertices = new mm::vec3[mesh->nVertices];
		loadedPhysicalVertexPositions.resize(mesh->nVertices);  // RT TMP
		for (u32 i = 0; i < mesh->nVertices; i++)
		{
			vertices[i] = *(mm::vec3*)((u8*)mesh->vertexBuffers[0] + i * mesh->vertexSize);
			loadedPhysicalVertexPositions[i] = vertices[i]; // RT TMP
		}
			
	}

	if (mass == 0)
		rigidEntity = physicsEngine->AddEntityRigidStatic(vertices, mesh->nVertices, mesh->indices, mesh->indexSize, mesh->nIndices);
	else
		rigidEntity = physicsEngine->AddEntityRigidDynamic(vertices, mesh->nVertices, mass);

	//loadedPhysicalVertexPositions = vertices;
	//delete vertices;
	//vertices = nullptr; // Important

	auto c = new RigidBodyComponent(rigidEntity);
	worldComponents.push_back(c);
	return c;
}

RigidBodyComponent* EngineCore::SpawnComp_RigidBodyCapsule(float height, float radius, float mass /* = 0*/)
{
	auto capsuleEntity = physicsEngine->AddEntityRigidCapsule(height, radius, mass);

	auto c = new RigidBodyComponent(capsuleEntity);
	worldComponents.push_back(c);
	return c;
}

CameraComponent* EngineCore::SpawnComp_Camera()
{
	auto c = new CameraComponent(graphicsEngine->CreateCam());
	worldComponents.push_back(c);
	return c;
}

Transform3DComponent* EngineCore::SpawnComp_Transform3D()
{
	auto c = new Transform3DComponent();
	worldComponents.push_back(c);
	return c;
}

void EngineCore::SetLayerCollision(size_t ID0, size_t ID1, bool bEnableCollision)
{
	assert(physicsEngine);
	physicsEngine->SetLayerCollision(ID0, ID1, bEnableCollision);
}

void EngineCore::Update(float deltaTime)
{
	// TODO: Physics engine debug draw
	//mm::vec3* linesFromNonUniqPoints_out;
	//size_t nLines_out;
	//if (physicsEngine->GetDebugData(linesFromNonUniqPoints_out, nLines_out))
	//{
	//
	//}


	// Destroy actors queued for destroying
	{
		PROFILE_SCOPE("Destroying Actors");
		for (auto& a : actorsToDestroy)
		{
			// Remove from actors
			auto it = std::find(actors.begin(), actors.end(), a);
			if (it != actors.end())
				actors.erase(it);

			// Remove from prevFrameCollision datas (Actor*)
			auto it2 = prevFrameActorCollideList.begin();
			while (it2 != prevFrameActorCollideList.end())
			{
				if (it2->first == a)
				{
					it2 = prevFrameActorCollideList.erase(it2);
					continue;
				}

				it2++;
			}

			for (auto& comp : a->GetComponents())
				Destroy(comp);

			delete a;
		}
		actorsToDestroy.clear();
	}
	
	{
		PROFILE_SCOPE("Modules Update");

		// Update physics
		if (physicsEngine)
		{
			PROFILE_SCOPE("Physics");
			physicsEngine->Update(deltaTime);
		}

		// Update components after physics simulation
		{
			PROFILE_SCOPE("Components Update After Physics");
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
	}

	// Update game logic
	{
		PROFILE_SCOPE("Game Logic");

		// Collision, enter, exit calls
		if(physicsEngine)
		{
			PROFILE_SCOPE("Core & onCollision(Enter, Exit, ...)");

			const std::vector<physics::rCollision>& collisionList = physicsEngine->GetCollisionList();

			curFrameActorCollideList.clear();

			for (auto& collision : collisionList)
			{
				// Fill up rCollision, based on rPhysicsCollision
				rCollision colData;

				// A
				if (collision.rigidBodyA)
				{
					colData.softBodyA = nullptr;
					colData.actorA = (Actor*)collision.rigidBodyA->GetUserPointer();

					if (colData.actorA)
					{
						// Search for component associated with RigidBodyEntity
						// Todo: optimize
						std::vector<RigidBodyComponent*> rigidBodyComps = colData.actorA->GetComponents<RigidBodyComponent>();
						for (auto& a : rigidBodyComps)
						{
							if (a->GetEntity() == collision.rigidBodyA)
							{
								colData.rigidBodyA = a;
								break;
							}
						}
					}
					else if (colData.actorA->IsKilled())
					{
						colData.actorA = nullptr;
					}
				}
				else if (collision.softBodyA)
				{
					colData.rigidBodyA = nullptr;
					colData.actorA = (Actor*)collision.softBodyA->GetUserPointer();

					if (colData.actorA)
					{
						// Search for component associated with RigidBodyEntity
						// Todo: optimize
						std::vector<SoftBodyComponent*> softBodyComps = colData.actorA->GetComponents<SoftBodyComponent>();
						for (auto& a : softBodyComps)
						{
							if (a->GetEntity() == collision.softBodyA)
							{
								colData.softBodyA = a;
								break;
							}
						}
					}
					else if (colData.actorA->IsKilled())
					{
						colData.actorA = nullptr;
					}
				}
				else
				{
					colData.actorA = nullptr;
				}

				// B
				if (collision.rigidBodyB)
				{
					colData.softBodyB = nullptr;
					colData.actorB = (Actor*)collision.rigidBodyB->GetUserPointer();

					if (colData.actorB)
					{
						// Search for component associated with RigidBodyEntity
						// Todo: optimize
						std::vector<RigidBodyComponent*> rigidBodyComps = colData.actorB->GetComponents<RigidBodyComponent>();
						for (auto& a : rigidBodyComps)
						{
							if (a->GetEntity() == collision.rigidBodyB)
							{
								colData.rigidBodyB = a;
								break;
							}
						}
					}
					else if (colData.actorB->IsKilled())
					{
						colData.actorB = nullptr;
					}
				}
				else if (collision.softBodyB)
				{
					colData.rigidBodyB = nullptr;
					colData.actorB = (Actor*)collision.softBodyB->GetUserPointer();

					if (colData.actorB)
					{
						// Search for component associated with RigidBodyEntity
						// Todo: optimize
						std::vector<SoftBodyComponent*> softBodyComps = colData.actorB->GetComponents<SoftBodyComponent>();
						for (auto& a : softBodyComps)
						{
							if (a->GetEntity() == collision.softBodyB)
							{
								colData.softBodyB = a;
								break;
							}
						}
					}
					else if (colData.actorB->IsKilled())
					{
						colData.actorB = nullptr;
					}
				}
				else
				{
					colData.actorB = nullptr;
				}

				if ((size_t)colData.actorA | (size_t)colData.actorB)
					colData.contacts = collision.contacts;
				else
					continue;

				auto CheckDispatchActorCollision = [&](Actor* a)
				{
					curFrameActorCollideList[a] = colData;

					// YES cur frame data (OnCollision)
					if (a->GetOnCollision())
						a->GetOnCollision()(colData);

					// If previous frame collided actor not found in current list, then Call OnCollisionExit
					bool bActorFound = false;
					for (auto& aPrev : prevFrameActorCollideList)
					{
						if (aPrev.first != a || aPrev.first->IsKilled())
							continue;

						bActorFound = true;

						const auto& collision = aPrev.second;
						if (collision.actorA == aPrev.first && collision.actorA->GetOnCollisionExit() != nullptr)
							collision.actorA->GetOnCollisionExit()(collision);
						else if (collision.actorB == aPrev.first && collision.actorB->GetOnCollisionExit() != nullptr)
							collision.actorB->GetOnCollisionExit()(collision);
					}

					// NO prev frame data, YES cur frame data for actor (OnCollisionEnter)
					if (!bActorFound && a->GetOnCollisionEnter())
						a->GetOnCollisionEnter()(colData);
				};

				if (colData.actorA)
					CheckDispatchActorCollision(colData.actorA);

				if (colData.actorB)
					CheckDispatchActorCollision(colData.actorB);
			}

			prevFrameActorCollideList = curFrameActorCollideList;
		}


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


		// IMPORTANT: TODO, if we A parent, B child actor attached together, then actorA and actorB will have the same component in the tree
		// So if B have rigidComponent colliding, ActorA also receive OnCollisionEvent, i don't know if it's expected result...
		{
			PROFILE_SCOPE("ActorLambda onUpdate");
			for (auto& a : actors)
				if (!a->IsKilled() && a->GetOnUpdate())
					a->GetOnUpdate()(deltaTime);
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
			for (i32 i = (i32)indicesToDelete.size(); --i >= 0;)
				tasks.erase(tasks.begin() + indicesToDelete[i]);
		}
	}

	// Profiling engine
#ifdef PROFILE_ENGINE
	VisualCpuProfiler::UpdateAndPresent();
#endif

	// Present opengl window
	graphicsEngine->GetTargetWindow()->Present();
}