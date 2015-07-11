// IWindow, IEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once

#include <GraphicsEngine/Raster/GraphicsEngineRaster.h>
#include <GraphicsEngine/RT/GraphicsEngineRT.h>
#include <PhysicsEngine/Bullet/PhysicsEngineBullet.h>
#include <SoundEngine/SFML/SoundEngineSFML.h>
#include <NetworkEngine/Boost/NetworkEngineBoost.h>

#include <GraphicsApi/IGapi.h>

class Factory
{
public:
	static graphics::IEngine*	CreateGraphicsEngineRaster(const rGraphicsEngineRasterData& d);
	static graphics::IEngine*	CreateGraphicsEngineRT(const rGraphicsEngineRT& d);
	static physics::IEngine*	CreatePhysicsEngineBullet(const rPhysicsEngineBullet& d);
	static network::IEngine*	CreateNetworkEngine(const rNetworkEngine& d);
	static sound::IEngine*		CreateSoundEngine(const rSoundEngine& d);

	static IGapi*				CreateGapiGL();
};