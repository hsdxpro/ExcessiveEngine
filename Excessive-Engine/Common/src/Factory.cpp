#include "Factory.h"
#include <locale>
#include <codecvt>

#ifdef	WIN
	#include "../CommonWin/src/Window.h"
#elif	LIN
	#include "../CommonLin/src/Window.h"
#elif	MAC
	#include "../CommonMac/src/Window.h"
#endif

#ifdef BUILD_STATIC
	#include "../GraphicsEngineRaster/src/GraphicsEngine.h"
	#include "../GraphicsEngineRT/src/GraphicsEngine.h"
	#include "../GraphicsApiGL/src/GapiGL.h"
#endif

graphics::IGraphicsEngine* Factory::createGraphicsEngineRaster(const rGraphicsEngineRaster& d) {
#ifdef BUILD_DLL
	using CreateT = graphics::IGraphicsEngine*(*)(const rGraphicsEngineRaster& d);
	auto module = Sys::loadDLL(L"GraphicsEngineRaster");
	if (!module) {
		return nullptr;
	}
	CreateT CreateGraphicsEngine = (CreateT)Sys::getDllProcAddress(module, "CreateGraphicsEngineRaster");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#elif BUILD_STATIC	
	return new GraphicsEngineRaster(d);
#endif
}

graphics::IGraphicsEngine* Factory::createGraphicsEngineRT(const rGraphicsEngineRT& d) {
#ifdef BUILD_DLL
	using CreateT = graphics::IGraphicsEngine*(*)(const rGraphicsEngineRT& d);
	auto module = Sys::loadDLL(L"GraphicsEngineRasterRT");
	if (!module) {
		return nullptr;
	}
	CreateT CreateGraphicsEngine = (CreateT)Sys::getDllProcAddress(module, "CreateGraphicsEngineRT");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#elif BUILD_STATIC	
	return new GraphicsEngineRT(d);
#endif
}

physics::IPhysicsEngine* Factory::createPhysicsEngineBullet(const rPhysicsEngineBullet& d) {
#ifdef BUILD_DLL
	using CreateT = physics::IPhysicsEngine*(*)(const rPhysicsEngineBullet& d);
	auto module = Sys::loadDLL(L"PhysicsEngineBullet");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::getDllProcAddress(module, "CreatePhysicsEngineBullet");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#elif BUILD_STATIC	
	return new PhysicsEngineBullet(d);
#endif
}

network::INetworkEngine* Factory::createNetworkEngine(const rNetworkEngine& d) {
#ifdef BUILD_DLL
	using CreateT = network::INetworkEngine*(*)(const rNetworkEngine& d);
	auto module = Sys::loadDLL(L"NetworkEngine");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::getDllProcAddress(module, "CreateNetworkEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#elif BUILD_STATIC	
	return new NetworkEngine(d);
#endif
}

sound::ISoundEngine* Factory::createSoundEngine(const rSoundEngine& d) {
#ifdef BUILD_DLL
	using CreateT = sound::ISoundEngine*(*)(const rSoundEngine& d);
	auto module = Sys::loadDLL(L"SoundEngine");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::getDllProcAddress(module, "CreateSoundEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#elif BUILD_STATIC	
	return new PhysicsEngineBullet(d);
#endif
}

IGapi* Factory::createGapiGL() {
#ifdef BUILD_DLL
	return ((IGapi*(*)())Sys::getDllProcAddress(Sys::loadDLL((Sys::getWorkDir() + L"GraphicsApiGL").c_str()), "createGraphicsApi"))();
#elif BUILD_STATIC
	return (IGapi*)new GapiGL();
#endif
}

IWindow* Factory::createWindow(const rWindow& d) {
	return new Window(d);
}