#pragma once
// The purpose of this class is to take the most minimal input from the user to (startup, use) engine

// Modules
#include "GraphicsEngine\IGraphicsEngine.h"
#include "PhysicsEngine\IPhysicsEngine.h"
#include "NetworkEngine\INetworkEngine.h"
#include "SoundEngine\ISoundEngine.h"
#include "GraphicsEngine_Raster\RasterGraphicsEngine.h"
//#include "GraphicsEngine_RT\GraphicsEngineRT.h"
#include "PhysicsEngine_Bullet\PhysicsEngineBullet.h"
#include "NetworkEngine_RakNet\NetworkEngineRakNet.h"
#include "SoundEngine_SFML\SoundEngineSFML.h"

#include "Actor.h"
#include "ActorScript.h"
#include "MeshComponent.h"
#include "RigidBodyComponent.h"
#include "CameraComponent.h"
#include "Transform3DComponent.h"

#include "SupportLibrary\Importer3D.h"

#include <unordered_map>
#include <vector>
#include <functional>
class Script;

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	// Init raster graphics engine, if one already exists will be destroyed, then instantiate it
	IGraphicsEngine* InitRasterGraphicsEngine(const rRasterGraphicsEngine& d = rRasterGraphicsEngine());

	// Init raytracer graphics engine, if one already exists will be destroyed, then instantiate it
	//IGraphicsEngine* InitGraphicsEngineRT(const rGraphicsEngineRT& d = rGraphicsEngineRT());
	
	// Init physics engine, if one already exists will be destroyed, then instantiate it
	IPhysicsEngine* InitPhysicsEngineBullet(const rPhysicsEngineBullet& d = rPhysicsEngineBullet());
	
	// Init network engine, if one already exists will be destroyed, then instantiate it
	INetworkEngine* InitNetworkEngineRakNet(const rNetworkEngine& d = rNetworkEngine());
	
	// Init network engine, if one already exists will be destroyed, then instantiate it
	ISoundEngine* InitSoundEngineSFML(const rSoundEngine& d = rSoundEngine());
	
	bool TraceClosestPoint_Physics(const mm::vec3& from, const mm::vec3& to, rPhysicsTraceResult& traceResult_out, const rPhysicsTraceParams& params = rPhysicsTraceParams());

	// Todo
	//bool TraceClosestPoint_Graphics(const mm::vec3& from, const mm::vec3& to, rTraceResult& traceInfo_out);
	
	sound::IEmitter* CreateSoundMono(const std::string& filePath, float volumeNormedPercent = 1, bool bLoop = false);
	
	Actor* SpawnActor(WorldComponent* rootComp = new Transform3DComponent());
	Actor* SpawnActor_MeshFromFile(const std::string& modelFilePath);
	Actor* SpawnActor_RigidBodyFromFile(const std::string& modelFilePath, float mass);
	Actor* SpawnActor_RigidBodyCapsule(float height, float radius, float mass = 0);
	Actor* SpawnActor_Camera();
	
	void Destroy(Actor* a);
	void Destroy(WorldComponent* c);

	void AddTask(const std::function<void()>& callb, float timeToProceed);
	
	template<class ScriptClass>
	Script* AddScript()
	{
		ScriptClass* p = new ScriptClass();
		scripts.push_back(p);
		return p;
	}
	
	template<class ActorScriptClass>
	ActorScript* AddActorScript()
	{
		ActorScriptClass* p = new ActorScriptClass();
		p->SetEntity(Core.AddEntity());
	
		entityScripts.push_back(p);
		return p;
	}
	
	MeshComponent*			SpawnComp_MeshFromFile(const std::string& modelFilePath);
	RigidBodyComponent*		SpawnComp_RigidBodyFromFile(const std::string& modelFilePath, float mass);
	RigidBodyComponent*		SpawnComp_RigidBodyCapsule(float height, float radius, float mass = 0);
	CameraComponent*		SpawnComp_Camera();
	Transform3DComponent*	SpawnComp_Transform3D();
	
	void SetLayerCollision(size_t ID0, size_t ID1, bool bEnableCollision);
	
	__inline void SetCam(CameraComponent* c)
	{ 
		assert(defaultGraphicsScene); 
		defaultGraphicsScene->SetCamera(c->GetCam()); 
	}
	
	void Update(float deltaTime);
	
	__inline Window* GetTargetWindow() 
	{
		assert(graphicsEngine);
		return graphicsEngine->GetTargetWindow();
	}
	
	//_inline IGraphicsEngine*	GetGraphicsEngine() const { return graphicsEngine; }
	//_inline IPhysicsEngine*		GetPhysicsEngine() const { return physicsEngine; }
	//_inline INetworkEngine*		GetNetworkEngine() const { return networkEngine; }
	//_inline ISoundEngine*		GetSoundEngine() const { return soundEngine; }

protected:
	IGraphicsEngine*	graphicsEngine;
	IPhysicsEngine*		physicsEngine;
	INetworkEngine*		networkEngine;
	ISoundEngine*		soundEngine;

	// Scripts
	std::vector<Script*> scripts;

	// Actors
	std::vector<Actor*> actors;
	std::vector<Actor*> actorsToDestroy;

	// Prev and cur frame actors associated collision data
	std::unordered_map<Actor*, rCollision> curFrameActorCollideList;
	std::unordered_map<Actor*, rCollision> prevFrameActorCollideList;

	// Entity scripts
	std::vector<ActorScript*> entityScripts;

	// World components
	std::vector<WorldComponent*> worldComponents;

	// Tasks
	std::vector<rTask> tasks;

	// Imported models
	std::unordered_map<std::string, rImporter3DData*> importedModels;

	// Imported mono sounds
	std::unordered_map<std::string, rMonoSound> importedSounds;

	// Imported textures...
	std::unordered_map<std::string, graphics::ITexture*> importedTextures;

	// The default graphicsScene Core created for us to spawn graphics things into
	graphics::IScene* defaultGraphicsScene;

	// The default soundScene Core created for us to spawn sound things into
	sound::IScene* defaultSoundScene;

	// Error diffuse texture for failed texture loads
	graphics::ITexture* texError;


// TMP !! Richard RT OpenCL Investigation, simple query of loaded physical vertex positions...
public:
	std::vector<mm::vec3> loadedPhysicalVertexPositions;
};

extern EngineCore Core;