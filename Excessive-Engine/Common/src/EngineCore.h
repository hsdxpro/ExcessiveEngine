#pragma once
// The purpose of this class is to take the most minimal input from the user to (startup, use) engine

// Modules

//#include "Actor.h"
#include "../GraphicsEngine/Raster/src/GraphicsEngineRaster.h"
#include "../GraphicsEngine/RT/src/GraphicsEngineRT.h"
#include "../PhysicsEngine/Bullet/src/PhysicsEngineBullet.h"
#include "../NetworkEngine/Boost/src/NetworkEngineBoost.h"
#include "../SoundEngine/SFML/src/SoundEngineSFML.h"
#include <unordered_map>

// These includes are for ComponentConstructor
#include "Importer3D.h"
#include "Actor.h"
#include "ComponentGraphics.h"
#include "ComponentRigidBody.h"
#include "ComponentCamera.h"

class EngineCore
{
public:
	// Nearly do nothing, null out vars
	EngineCore();
	~EngineCore();

	// Init raster graphics engine, if one already exists will be destroyed, then instantiate it
	graphics::IEngine* initGraphicsEngineRaster(const rGraphicsEngineRaster& d = rGraphicsEngineRaster());

	// Init raytracer graphics engine, if one already exists will be destroyed, then instantiate it
	graphics::IEngine* initGraphicsEngineRT(const rGraphicsEngineRT& d = rGraphicsEngineRT());

	// Init physics engine, if one already exists will be destroyed, then instantiate it
	physics::IEngine* initPhysicsEngineBullet(const rPhysicsEngineBullet& d = rPhysicsEngineBullet());

	// Init network engine, if one already exists will be destroyed, then instantiate it
	network::IEngine* initNetworkEngine(const rNetworkEngine& d = rNetworkEngine());

	// Init network engine, if one already exists will be destroyed, then instantiate it
	sound::IEngine* initSoundEngine(const rSoundEngine& d = rSoundEngine());
	
	Actor* addActor();

	ComponentGraphics*  addCompGraphicsFromFile(const std::wstring& modelFilePath);
	ComponentRigidBody* addCompRigidBodyFromFile(const std::wstring& modelFilePath, float mass);
	ComponentRigidBody* addCompRigidBodyCapsule(float height, float radius, float mass = 0);
	ComponentCamera*	addCompCamera();

	graphics::IMaterial* createGraphicsMaterial();

	void update(float deltaTime);

	void setCam(ComponentCamera* c);

	graphics::IEngine*	getGraphicsEngine();
	physics::IEngine*	getPhysicsEngine();
	network::IEngine*	getNetworkEngine();
	sound::IEngine*		getSoundEngine();

	graphics::IScene*	getDefaultGraphicsScene();

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