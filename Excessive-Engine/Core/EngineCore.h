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

#include "Actor.h"
#include "GraphicsComponent.h"
#include "RigidBodyComponent.h"
#include "CameraComponent.h"
#include "SupportLibrary\Importer3D.h"

//#include "Actor.h"
#include <unordered_map>



class EngineCore
{
public:
	// Nearly do nothing, null out vars
	EngineCore();
	~EngineCore();

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
	
	Actor* AddActor();

	GraphicsComponent*  AddCompGraphicsFromFile(const std::wstring& modelFilePath);
	RigidBodyComponent* AddCompRigidBodyFromFile(const std::wstring& modelFilePath, float mass);
	RigidBodyComponent* AddCompRigidBodyCapsule(float height, float radius, float mass = 0);
	CameraComponent*	AddCompCamera();

	graphics::IMaterial* CreateGraphicsMaterial();

	void Update(float deltaTime);

	void SetCam(CameraComponent* c);

	graphics::IEngine*	GetGraphicsEngine();
	physics::IEngine*	GetPhysicsEngine();
	network::IEngine*	GetNetworkEngine();
	sound::IEngine*		GetSoundEngine();

	graphics::IScene*	GetDefaultGraphicsScene();

protected:
	graphics::IEngine*	graphicsEngine;
	physics::IEngine*	physicsEngine;
	network::IEngine*	networkEngine;
	sound::IEngine*		soundEngine;

	graphics::IScene*	defaultGraphicsScene;

	std::vector<Actor*> actors;
	std::vector<WorldComponent*> worldComponents;

	std::unordered_map<std::wstring, rImporter3DData*> importedModels;

	// Error diffuse texture for failed texture loads
	graphics::ITexture* texError;
};