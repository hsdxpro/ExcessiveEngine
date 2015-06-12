// IWindow, IEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once

#include "../../GraphicsEngine/Raster/src/GraphicsEngineRaster.h"
#include "../../GraphicsEngine/RT/src/GraphicsEngineRT.h"
#include "../../PhysicsEngine/Bullet/src/PhysicsEngineBullet.h"
#include "../../SoundEngine/SFML/src/SoundEngineSFML.h"
#include "../../NetworkEngine/Boost/src/NetworkEngineBoost.h"

#include "IWindow.h"
#include "ITimer.h"
#include "../GraphicsApi/Interfaces/IGapi.h"

class Factory
{
public:
	static graphics::IEngine*	CreateGraphicsEngineRaster(const rGraphicsEngineRaster& d);
	static graphics::IEngine*	CreateGraphicsEngineRT(const rGraphicsEngineRT& d);
	static physics::IEngine*	CreatePhysicsEngineBullet(const rPhysicsEngineBullet& d);
	static network::IEngine*	CreateNetworkEngine(const rNetworkEngine& d);
	static sound::IEngine*		CreateSoundEngine(const rSoundEngine& d);

	static IWindow*				CreateWindow(const struct rWindow& d);
	static ITimer*				CreateTimer();
	static IGapi*				CreateGapiGL();
};