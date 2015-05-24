// IWindow, IEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once

#include "../../GraphicsEngine/Raster/src/GraphicsEngineRaster.h"
#include "../../GraphicsEngine/RT/src/GraphicsEngineRT.h"
#include "../../PhysicsEngine/Bullet/src/PhysicsEngineBullet.h"
#include "../../SoundEngine/SFML/src/SoundEngineSFML.h"
#include "../../NetworkEngine/Boost/src/NetworkEngineBoost.h"

#include "IWindow.h"
class ITimer;
class IGapi;

class Factory
{
public:
	static graphics::IEngine*	createGraphicsEngineRaster(const rGraphicsEngineRaster& d);
	static graphics::IEngine*	createGraphicsEngineRT(const rGraphicsEngineRT& d);
	static physics::IEngine*	createPhysicsEngineBullet(const rPhysicsEngineBullet& d);
	static network::IEngine*	createNetworkEngine(const rNetworkEngine& d);
	static sound::IEngine*		createSoundEngine(const rSoundEngine& d);

	static IWindow*				createWindow(const struct rWindow& d);
	static ITimer*				createTimer();
	static IGapi*				createGapiGL();
};