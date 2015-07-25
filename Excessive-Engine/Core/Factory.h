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
	static IGraphicsEngine*	CreateGraphicsEngineRaster(const rGraphicsEngineRasterData& d);
	static IGraphicsEngine*	CreateGraphicsEngineRT(const rGraphicsEngineRT& d);
	static IPhysicsEngine*	CreatePhysicsEngineBullet(const rPhysicsEngineBullet& d);
	static INetworkEngine*	CreateNetworkEngine(const rNetworkEngine& d);
	static ISoundEngine*	CreateSoundEngine(const rSoundEngine& d);

	static IGapi*				CreateGapiGL();
};