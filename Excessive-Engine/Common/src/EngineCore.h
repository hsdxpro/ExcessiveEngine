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

struct rImporter3DData;

class EngineCore;

template<class ConstructedType>
class ComponentConstructor;

class EngineCoreBaseConstructor
{
public:
	EngineCoreBaseConstructor(EngineCore* e, WorldComponent* comp = nullptr, WorldComponent* parentComp = nullptr, WorldComponent* rootComponent = nullptr)
	:engineCore(e), constructedComp(comp), rootComponent(rootComponent)
	{
	}

	EngineCoreBaseConstructor* addCompGraphicsFromFile(const std::wstring& modelFilePath, graphics::IScene* scene = nullptr);
	EngineCoreBaseConstructor* addCompRigidBodyFromFile(const std::wstring& modelFilePath, float mass);


	template<class T>
	T* getRootComp() { return static_cast<T*>(rootComponent); }

protected:
	EngineCore*			engineCore;
	WorldComponent*		constructedComp;
	WorldComponent*		rootComponent;
};

template<class ConstructedType>
class ComponentConstructor : public EngineCoreBaseConstructor
{
public:
	ComponentConstructor(EngineCore* e, ConstructedType* comp = nullptr, WorldComponent* parentComp = nullptr, WorldComponent* rootComponent = nullptr)
	:EngineCoreBaseConstructor(e,comp,parentComp, rootComponent)
	{
		if (comp)
		{
			// Root component !
			if (!parentComp)
			{
				engineCore->GetRootComponents().push_back(comp);
			}
			else
				parentComp->addChild(comp);
		}
	}
};

class EngineCore : public EngineCoreBaseConstructor
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
	
	// DEPRECATED !!!!
	//Actor* addActor();

	void update(float deltaTime/*, graphics::IScene* scene*/);

	graphics::IEngine*	getGraphicsEngine();
	physics::IEngine*	getPhysicsEngine();
	network::IEngine*	getNetworkEngine();
	sound::IEngine*		getSoundEngine();

	graphics::ITexture* getTexError();
	graphics::IScene*	getDefaultGraphicsScene();

	std::unordered_map<std::wstring, rImporter3DData*>& getImportedModels();

	std::vector<WorldComponent*>& GetRootComponents();

protected:
	graphics::IEngine*	graphicsEngine;
	physics::IEngine*	physicsEngine;
	network::IEngine*	networkEngine;
	sound::IEngine*		soundEngine;

	graphics::IScene*	defaultGraphicsScene;

	std::vector<WorldComponent*> rootComponents;

	std::unordered_map<std::wstring, rImporter3DData*> importedModels;

	// Error diffuse texture for failed texture loads
	graphics::ITexture* texError;
};