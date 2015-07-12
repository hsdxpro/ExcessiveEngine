#pragma once
// The purpose of this class is to take the most minimal input from the user to (startup, use) engine

// Modules
#include "GraphicsEngine\IEngine.h"
#include "PhysicsEngine\IEngine.h"
#include "NetworkEngine\IEngine.h"
#include "SoundEngine\IEngine.h"
#include "GraphicsEngine\Raster\GraphicsEngineRaster.h"
#include "GraphicsEngine\RT\GraphicsEngineRT.h"
#include "PhysicsEngine\Bullet\PhysicsEngineBullet.h"
#include "NetworkEngine\Boost\NetworkEngineBoost.h"
#include "SoundEngine\SFML\SoundEngineSFML.h"

#include "Script.h"
#include "Thing.h"
#include "ActorScript.h"
#include "Actor.h"
#include "GraphicsComponent.h"
#include "RigidBodyComponent.h"
#include "CameraComponent.h"

#include "SupportLibrary\Importer3D.h"

#include <unordered_map>
#include <vector>



class Core
{
public:
	// Nearly do nothing, null out vars
	Core();
	~Core();

	// Init raster graphics engine, if one already exists will be destroyed, then instantiate it
	graphics::IEngine* InitGraphicsEngineRaster(const rGraphicsEngineRaster& d = rGraphicsEngineRaster());

	// Init raytracer graphics engine, if one already exists will be destroyed, then instantiate it
	graphics::IEngine* InitGraphicsEngineRT(const rGraphicsEngineRT& d = rGraphicsEngineRT());

	// Init physics engine, if one already exists will be destroyed, then instantiate it
	physics::IEngine* InitPhysicsEngineBullet(const rPhysicsEngineBullet& d = rPhysicsEngineBullet());

	// Init network engine, if one already exists will be destroyed, then instantiate it
	network::IEngine* InitNetworkEngine(const rNetworkEngine& d = rNetworkEngine());

	// Init network engine, if one already exists will be destroyed, then instantiate it
	sound::IEngine* InitSoundEngine(const rSoundEngine& d = rSoundEngine());

	// TODO!
	//ThingType*			  CreateThingType(Thing* t);
	//GraphicsComponentType*  CreateCompGraphicsType(GraphicsComponent* comp);
	//RigidBodyComponentType* CreateCompRigidBodyType(RigidBodyComponent* comp);
	//CameraComponentType*	  CreateCompCameraType(CameraComponent* comp);

	Thing* SpawnThing(ActorScript* s);

	Actor* AddActor();

	template<class ScriptClass>
	Script* AddScript();

	template<class ActorScriptClass>
	ActorScript* AddActorScript();

	//Actor* AddActor();
	GraphicsComponent*  SpawnCompGraphicsFromFile(const std::wstring& modelFilePath);
	RigidBodyComponent* SpawnCompRigidBodyFromFile(const std::wstring& modelFilePath, float mass);
	RigidBodyComponent* SpawnCompRigidBodyCapsule(float height, float radius, float mass = 0);
	CameraComponent*	SpawnCompCamera();

	void SetCam(CameraComponent* c);
	void Update(float deltaTime);

	IWindow* GetTargetWindow();

	graphics::IEngine*	GetGraphicsEngine();
	physics::IEngine*	GetPhysicsEngine();
	network::IEngine*	GetNetworkEngine();
	sound::IEngine*		GetSoundEngine();

protected:
	graphics::IEngine*	graphicsEngine;
	physics::IEngine*	physicsEngine;
	network::IEngine*	networkEngine;
	sound::IEngine*		soundEngine;

	// Scripts
	std::vector<Script*> scripts;

	// Things
	std::vector<Thing*> things;

	// Actor scripts
	std::vector<ActorScript*> actorScripts;

	// Actors
	std::vector<Actor*> actors;

	// World components
	std::vector<WorldComponent*> worldComponents;

	// Imported models
	std::unordered_map<std::wstring, rImporter3DData*> importedModels;

	// The default graphicsScene we add things to
	graphics::IScene* defaultGraphicsScene;

	// Error diffuse texture for failed texture loads
	graphics::ITexture* texError;
};

extern Core gCore;

template<class ScriptClass>
Script* Core::AddScript()
{
	ScriptClass* p = new ScriptClass();
		scripts.push_back(p);
	return p;
}

template<class ActorScriptClass>
ActorScript* Core::AddActorScript()
{
	ActorScriptClass* p = new ActorScriptClass(gCore.AddActor());
		actorScripts.push_back(p);
	return p;
}