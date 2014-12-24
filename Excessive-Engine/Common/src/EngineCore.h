#pragma once
// The purpose of this class is to take the most minimal input from the user to (startup, use) engine

// Modules
#include "..\GraphicsEngine_Interface\interface\IGraphicsEngine.h"
#include "..\GraphicsEngineRaster\src\GraphicsEngine.h"
#include "..\GraphicsEngineRT\src\GraphicsEngine.h"

class EngineCore
{
public:
	// Nearly do nothing, null out vars
	EngineCore();

	// Init graphics engine, if one already exists will be destroyed, then instantiate it
	graphics::IGraphicsEngine* initGraphicsEngineRaster(const rGraphicsEngineRaster& desc);

	// Init graphics engine, if one already exists will be destroyed, then instantiate it
	graphics::IGraphicsEngine* initGraphicsEngineRT(const rGraphicsEngineRT& desc);

	// Init physics engine, if one already exists will be destroyed, then instantiate it
	//bool initPhysicsEngine(const physics::rEngine& graphics);

	// Init network engine, if one already exists will be destroyed, then instantiate it
	//bool initNetworkEngine(const network::rEngine& graphics);

protected:
	graphics::IGraphicsEngine* graphicsEngine;
	//physics::IEngine* physicsEngine;
	//network::IEngine* networkEngine;
};