#pragma once
// The purpose of this class is to take the most minimal input from the user to (startup, use) engine

// Modules
#include "GraphicsEngine\IGraphicsEngine.h"
#include "PhysicsEngine\IPhysicsEngine.h"
#include "NetworkEngine\INetworkEngine.h"
#include "SoundEngine\ISoundEngine.h"
#include "GraphicsEngine\Raster\GraphicsEngineRaster.h"
#include "GraphicsEngine\RT\GraphicsEngineRT.h"
#include "PhysicsEngine\Bullet\PhysicsEngineBullet.h"
#include "NetworkEngine\Boost\NetworkEngineBoost.h"
#include "SoundEngine\SFML\SoundEngineSFML.h"

#include "Script.h"
#include "Actor.h"
#include "EntityScript.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include "RigidBodyComponent.h"
#include "CameraComponent.h"

#include "SupportLibrary\Importer3D.h"

#include <unordered_map>
#include <vector>
#include <functional>

struct rMonoSound
{
	sound::ISoundData* soundData;
	sound::IEmitter* soundEmitter;
};

struct rTask
{
	std::function<void()> callb;
	float timeLeft;
};

extern class Core* gCore;

class Core
{
public:
	// Nearly do nothing, null out vars
	static void Instantiate(){ if(!gCore)gCore = new Core(); }

	Core();
	~Core();

	// Init raster graphics engine, if one already exists will be destroyed, then instantiate it
	IGraphicsEngine* InitGraphicsEngineRaster(const rGraphicsEngineRaster& d = rGraphicsEngineRaster());

	// Init raytracer graphics engine, if one already exists will be destroyed, then instantiate it
	IGraphicsEngine* InitGraphicsEngineRT(const rGraphicsEngineRT& d = rGraphicsEngineRT());

	// Init physics engine, if one already exists will be destroyed, then instantiate it
	IPhysicsEngine* InitPhysicsEngineBullet(const rPhysicsEngineBullet& d = rPhysicsEngineBullet());

	// Init network engine, if one already exists will be destroyed, then instantiate it
	INetworkEngine* InitNetworkEngine(const rNetworkEngine& d = rNetworkEngine());

	// Init network engine, if one already exists will be destroyed, then instantiate it
	ISoundEngine* InitSoundEngineSFML(const rSoundEngine& d = rSoundEngine());

	// TODO!
	//ThingType*			  CreateThingType(Thing* t);
	//GraphicsComponentType*  CreateCompGraphicsType(GraphicsComponent* comp);
	//RigidBodyComponentType* CreateCompRigidBodyType(RigidBodyComponent* comp);
	//CameraComponentType*	  CreateCompCameraType(CameraComponent* comp);

	sound::IEmitter* CreateSoundMono(const std::string& filePath, float volumeNormedPercent = 1, bool bLoop = false);

	Actor* SpawnActor();
	Actor* SpawnActor(EntityScript* s);

	void DestroyActor(Actor* a);
	void DestroyComp(WorldComponent* c);

	Actor* SpawnActor_MeshFromFile(const std::string& modelFilePath);
	Actor* SpawnActor_RigidBodyFromFile(const std::string& modelFilePath, float mass);
	Actor* SpawnActor_RigidBodyCapsule(float height, float radius, float mass = 0);
	Actor* SpawnActor_Camera();

	void AddTask(const std::function<void()>& callb, float timeToProceed);

	Entity* AddEntity();

	template<class ScriptClass>
	Script* AddScript();

	template<class ActorScriptClass>
	EntityScript* AddEntityScript();

	GraphicsComponent*  SpawnComp_MeshFromFile(const std::string& modelFilePath);
	RigidBodyComponent* SpawnComp_RigidBodyFromFile(const std::string& modelFilePath, float mass);
	RigidBodyComponent* SpawnComp_RigidBodyCapsule(float height, float radius, float mass = 0);
	CameraComponent*	SpawnComp_Camera();

	bool SetLayerCollision(size_t ID0, size_t ID1, bool bEnableCollision);

	void SetCam(CameraComponent* c);

	void Update(float deltaTime);

	Window* GetTargetWindow();

	IGraphicsEngine*	GetGraphicsEngine();
	IPhysicsEngine*	GetPhysicsEngine();
	INetworkEngine*	GetNetworkEngine();
	ISoundEngine*		GetSoundEngine();

protected:
	IGraphicsEngine*	graphicsEngine;
	IPhysicsEngine*	physicsEngine;
	INetworkEngine*	networkEngine;
	ISoundEngine*		soundEngine;

	// Scripts
	std::vector<Script*> scripts;

	// Actors
	std::vector<Actor*> actors;
	std::vector<Actor*> actorsToDestroy;

	std::unordered_map<Actor*, Actor*> prevFrameActorCollideList;
	std::vector<rCollision> prevFrameActorCollisionData;

	// Entity scripts
	std::vector<EntityScript*> entityScripts;

	// Entities
	std::vector<Entity*> entities;

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
};

template<class ScriptClass>
Script* Core::AddScript()
{
	ScriptClass* p = new ScriptClass();
		scripts.push_back(p);
	return p;
}

template<class ActorScriptClass>
EntityScript* Core::AddEntityScript()
{
	ActorScriptClass* p = new ActorScriptClass();
	p->SetEntity(gCore->AddEntity());

	entityScripts.push_back(p);
	return p;
}