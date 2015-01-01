#pragma once
// The purpose of this class is to take the most minimal input from the user to (startup, use) engine

// Modules
#include "../GraphicsEngineRaster/src/GraphicsEngineRaster.h"
#include "../GraphicsEngineRT/src/GraphicsEngineRT.h"
#include "../PhysicsEngineBullet/src/PhysicsEngineBullet.h"
#include "../SoundEngine/src/SoundEngine.h"
#include "../NetworkEngine/src/NetworkEngine.h"

#include "Entity.h"

enum class ePhysicsCore {
	BULLET,
	//PHYSX,
};

enum class eGraphicsCore {
	RASTER,
	RAYTRACER,
};

enum class eSoundCore {
	SFML,
};

enum class eNetworCore {
	SFML,
	//RAKNET,
};

class EngineCore
{
public:
	// Nearly do nothing, null out vars
	EngineCore();
	~EngineCore();

	// Init graphics engine, if one already exists will be destroyed, then instantiate it
	graphics::IGraphicsEngine* initGraphicsEngineRaster(const rGraphicsEngineRaster& d = rGraphicsEngineRaster());

	// Init graphics engine, if one already exists will be destroyed, then instantiate it
	graphics::IGraphicsEngine* initGraphicsEngineRT(const rGraphicsEngineRT& d = rGraphicsEngineRT());

	// Init physics engine, if one already exists will be destroyed, then instantiate it
	physics::IPhysicsEngine* initPhysicsEngineBullet(const rPhysicsEngineBullet& d = rPhysicsEngineBullet());

	// Init network engine, if one already exists will be destroyed, then instantiate it
	network::INetworkEngine* initNetworkEngine(const rNetworkEngine& d = rNetworkEngine());

	sound::ISoundEngine* initSoundEngine(const rSoundEngine& d = rSoundEngine());
	
	// Create high level Entity, that encapsulates (graphics, phyiscs, sound, network, ....) entities
	// The input paths are files like .dae, .fbx etc
	Entity* createEntity(graphics::IScene* gScene, const std::wstring& modelPath);


	graphics::IGraphicsEngine*	getGraphicsEngine() { return graphicsEngine; }
	physics::IPhysicsEngine*	getPhysicsEngine() { return physicsEngine; }
	network::INetworkEngine*	getNetworkEngine() { return networkEngine; }
	sound::ISoundEngine*		getSoundEngine() { return soundEngine; }
	
protected:
	graphics::IGraphicsEngine*	graphicsEngine;
	physics::IPhysicsEngine*	physicsEngine;
	network::INetworkEngine*	networkEngine;
	sound::ISoundEngine*		soundEngine;

	std::vector<Entity*> entities;
};