// IWindow, IGraphicsEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once

#include "../GraphicsEngineRT/src/GraphicsEngineRT.h"
#include "../GraphicsEngineRaster/src/GraphicsEngineRaster.h"
#include "../PhysicsEngineBullet/src/PhysicsEngineBullet.h"
#include "../NetworkEngine/src/NetworkEngine.h"
#include "../SoundEngine/src/SoundEngine.h"

#include "../Module_Interfaces/GraphicsApi/IGapi.h"
#include "IWindow.h"

class Factory
{
public:
	static graphics::IGraphicsEngine*	createGraphicsEngineRaster(const rGraphicsEngineRaster& d);
	static graphics::IGraphicsEngine*	createGraphicsEngineRT(const rGraphicsEngineRT& d);
	static physics::IPhysicsEngine*		createPhysicsEngineBullet(const rPhysicsEngineBullet& d);
	static network::INetworkEngine*		createNetworkEngine(const rNetworkEngine& d);
	static sound::ISoundEngine*			createSoundEngine(const rSoundEngine& d);

	static IWindow*						createWindow(const struct rWindow& d);
	static IGapi*						createGapiGL();
};