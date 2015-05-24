#include "Factory.h"
#include <locale>
#include <codecvt>

#include "../GraphicsApi/GL/src/GapiGL.h"

#ifdef	WIN
	#include "../CommonWin/src/Window.h"
	#include "../CommonWin/src/Timer.h"
#elif	LIN
	#include "../CommonLin/src/Window.h"
	#include "../CommonLin/src/Timer.h"
#elif	MAC
	#include "../CommonMac/src/Window.h"
	#include "../CommonMac/src/Timer.h"
#endif

graphics::IEngine* Factory::createGraphicsEngineRaster(const rGraphicsEngineRaster& d) 
{
#ifdef BUILD_DLL
	using CreateT = graphics::IEngine*(*)(const rGraphicsEngineRaster& d);
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

graphics::IEngine* Factory::createGraphicsEngineRT(const rGraphicsEngineRT& d) 
{
#ifdef BUILD_DLL
	using CreateT = graphics::IEngine*(*)(const rGraphicsEngineRT& d);
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

physics::IEngine* Factory::createPhysicsEngineBullet(const rPhysicsEngineBullet& d) 
{
#ifdef BUILD_DLL
	using CreateT = physics::IEngine*(*)(const rPhysicsEngineBullet& d);
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

network::IEngine* Factory::createNetworkEngine(const rNetworkEngine& d) 
{
#ifdef BUILD_DLL
	using CreateT = network::IEngine*(*)(const rNetworkEngine& d);
	auto module = Sys::loadDLL(L"NetworkEngineBoost");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::getDllProcAddress(module, "CreateNetworkEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#elif BUILD_STATIC	
	return new NetworkEngineBoost(d);
#endif
}

sound::IEngine* Factory::createSoundEngine(const rSoundEngine& d) 
{
#ifdef BUILD_DLL
	using CreateT = sound::IEngine*(*)(const rSoundEngine& d);
	auto module = Sys::loadDLL(L"SoundEngineSFML");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::getDllProcAddress(module, "CreateSoundEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#elif BUILD_STATIC	
	return nullptr;// new SoundEngineSFML(d);
#endif
}

IGapi* Factory::createGapiGL() 
{
#ifdef BUILD_DLL
	return ((IGapi*(*)())Sys::getDllProcAddress(Sys::loadDLL((Sys::getWorkDir() + L"GraphicsApiGL").c_str()), "createGraphicsApi"))();
#elif BUILD_STATIC
	return (IGapi*)new GapiGL();
#endif
}

IWindow* Factory::createWindow(const rWindow& d) 
{
	return new Window(d);
}

ITimer* Factory::createTimer()
{
	return new Timer();
}