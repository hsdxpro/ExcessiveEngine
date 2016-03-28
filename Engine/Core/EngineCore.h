#pragma once
// The purpose of this class is to take the most minimal input from the user to (startup, use) the engine

// Modules
#include "GraphicsEngine\IGraphicsEngine.h"
#include "PhysicsEngine\IPhysicsEngine.h"
#include "NetworkEngine\INetworkEngine.h"
#include "SoundEngine\ISoundEngine.h"
#include "GraphicsEngine_Raster\GraphicsEngineRaster.h"
#include "PhysicsEngine_Bullet\PhysicsEngineBullet.h"
#include "NetworkEngine_RakNet\NetworkEngineRakNet.h"
#include "SoundEngine_SFML\SoundEngineSFML.h"

//#include "Actor.h"
#include "ActorScript.h"
#include "MeshComponent.h"
#include "RigidBodyComponent.h"
#include "CameraComponent.h"
#include "Transform3DComponent.h"

#include "SupportLibrary\Importer3D.h"
#include "GraphicsEngine_RT\GraphicsEngineRT.h"

#include <unordered_map>
#include <vector>
#include <functional>
#include "CoreCommon.h"

class Script;

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	// Init raster graphics engine, if one already exists will be destroyed, then instantiate it
	IGraphicsEngine* InitGraphicsEngineRaster(const GraphicsEngineRasterDesc& d = GraphicsEngineRasterDesc());

	// Init raster graphics engine, if one already exists will be destroyed, then instantiate it
	IGraphicsEngine* InitGraphicsEngineRasterZsiros(const GraphicsEngineRasterDesc& d = GraphicsEngineRasterDesc());

	// Init raytracer graphics engine, if one already exists will be destroyed, then instantiate it
	IGraphicsEngine* InitGraphicsEngineRT(const rGraphicsEngineRT& d = rGraphicsEngineRT());
	
	// Init physics engine, if one already exists will be destroyed, then instantiate it
	IPhysicsEngine* InitPhysicsEngineBullet(const PhysicsEngineBulletDesc& d = PhysicsEngineBulletDesc());
	
	// Init network engine, if one already exists will be destroyed, then instantiate it
	INetworkEngine* InitNetworkEngineRakNet(const rNetworkEngine& d = rNetworkEngine());
	
	// Init network engine, if one already exists will be destroyed, then instantiate it
	ISoundEngine* InitSoundEngineSFML(const rSoundEngine& d = rSoundEngine());
	
	bool TraceClosestPoint_Physics(const mm::vec3& from, const mm::vec3& to, PhysicsTraceResult& traceResult_out, const PhysicsTraceParams& params = PhysicsTraceParams());

	// Todo
	//bool TraceClosestPoint_Graphics(const mm::vec3& from, const mm::vec3& to, TraceResult& traceInfo_out);
	
	sound::IEmitter* CreateMonoSound(const std::string& filePath, float volumeNormedPercent = 1, bool bLoop = false);
	
	Actor* AddActor();
	Actor* AddActor(const std::string& modelFilePath, float mass = 0);
	Actor* AddActor(WorldComponent* rootComp);
	Actor* AddActor_Mesh(const std::string& modelFilePath);
	Actor* AddActor_RigidBody(const std::string& modelFilePath, float mass);
	Actor* AddActor_RigidBodyCapsule(float height, float radius, float mass = 0);
	Actor* AddActor_Camera();
	
	void Remove(Actor* a);
	void Remove(WorldComponent* c);

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
	
	MeshComponent*			AddComponent_Mesh(const std::string& modelFilePath);
	RigidBodyComponent*		AddComponent_RigidBody(const std::string& modelFilePath, float mass);
	RigidBodyComponent*		AddComponent_RigidBodyCapsule(float height, float radius, float mass = 0);
	CameraComponent*		AddComponent_Camera();
	Transform3DComponent*	AddComponent_Transform3D();
	
	void SetLayeCollision(size_t ID0, size_t ID1, bool bEnableCollision);
	
	inline void SetCam(CameraComponent* c)
	{ 
		assert(defaultGraphicsScene); 
		defaultGraphicsScene->SetCamera(c->GetCam()); 
	}
	
	void Update(float deltaTime);
	
	inline Window* GetTargetWindow() 
	{
		assert(graphicsEngine);
		return graphicsEngine->GetTargetWindow();
	}
	
	inline IGraphicsEngine*	GetGraphicsEngine() const { return graphicsEngine; }
	inline IPhysicsEngine*	GetPhysicsEngine() const { return physicsEngine; }
	inline INetworkEngine*	GetNetworkEngine() const { return networkEngine; }
	inline ISoundEngine*	GetSoundEngine() const { return soundEngine; }

	inline std::vector<WorldComponent*> GetWorldComponents(eWorldComponentType type);
	inline std::vector<WorldComponent*> GetWorldComponents();

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
	std::unordered_map<Actor*, Collision> curFrameActorCollideList;
	std::unordered_map<Actor*, Collision> prevFrameActorCollideList;

	// Entity scripts
	std::vector<ActorScript*> entityScripts;

	// World components
	std::vector<WorldComponent*> worldComponents;

	// Tasks
	std::vector<Task> tasks;

	// Imported models
	std::unordered_map<std::string, rImporter3DData*> importedModels;

	// Imported mono sounds
	std::unordered_map<std::string, MonoSound> importedSounds;

	// Imported textures...
	std::unordered_map<std::string, graphics::ITexture*> importedTextures;

	// The default graphicsScene Core created for us to spawn graphics things into
	graphics::IScene* defaultGraphicsScene;

	// The default soundScene Core created for us to spawn sound things into
	sound::IScene* defaultSoundScene;

	// Error diffuse texture for failed texture loads
	graphics::ITexture* texError;
};

extern EngineCore Core;

std::vector<WorldComponent*> EngineCore::GetWorldComponents()
{
	return worldComponents;
}

std::vector<WorldComponent*> EngineCore::GetWorldComponents(eWorldComponentType type)
{
	std::vector<WorldComponent*> result;
	//result.clear();

	for (WorldComponent* c : GetWorldComponents())
	{
		if (c->GetType() == type)
		{
			result.push_back(c);
		}
	}

	return result;
}