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

graphics::IEngine* Factory::CreateGraphicsEngineRaster(const rGraphicsEngineRaster& d) 
{
#ifdef BUILD_DLL
	using CreateT = graphics::IEngine*(*)(const rGraphicsEngineRaster& d);
	auto module = Sys::LoadDLL(L"GraphicsEngineRaster");
	if (!module) {
		return nullptr;
	}
	CreateT CreateGraphicsEngine = (CreateT)Sys::GetDLLProcAddress(module, "CreateGraphicsEngineRaster");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#elif BUILD_STATIC	
	return new GraphicsEngineRaster(d);
#endif
}

graphics::IEngine* Factory::CreateGraphicsEngineRT(const rGraphicsEngineRT& d) 
{
#ifdef BUILD_DLL
	using CreateT = graphics::IEngine*(*)(const rGraphicsEngineRT& d);
	auto module = Sys::LoadDLL(L"GraphicsEngineRasterRT");
	if (!module) {
		return nullptr;
	}
	CreateT CreateGraphicsEngine = (CreateT)Sys::GetDLLProcAddress(module, "CreateGraphicsEngineRT");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#elif BUILD_STATIC	
	return new GraphicsEngineRT(d);
#endif
}

physics::IEngine* Factory::CreatePhysicsEngineBullet(const rPhysicsEngineBullet& d) 
{
#ifdef BUILD_DLL
	using CreateT = physics::IEngine*(*)(const rPhysicsEngineBullet& d);
	auto module = Sys::LoadDLL(L"PhysicsEngineBullet");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::GetDLLProcAddress(module, "CreatePhysicsEngineBullet");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#elif BUILD_STATIC	
	return new PhysicsEngineBullet(d);
#endif
}

network::IEngine* Factory::CreateNetworkEngine(const rNetworkEngine& d) 
{
#ifdef BUILD_DLL
	using CreateT = network::IEngine*(*)(const rNetworkEngine& d);
	auto module = Sys::LoadDLL(L"NetworkEngineBoost");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::GetDLLProcAddress(module, "CreateNetworkEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#elif BUILD_STATIC	
	return new NetworkEngineBoost(d);
#endif
}

sound::IEngine* Factory::CreateSoundEngine(const rSoundEngine& d) 
{
#ifdef BUILD_DLL
	using CreateT = sound::IEngine*(*)(const rSoundEngine& d);
	auto module = Sys::LoadDLL(L"SoundEngineSFML");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::GetDLLProcAddress(module, "CreateSoundEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#elif BUILD_STATIC	
	return new SoundEngineSFML(d);
#endif
}

IGapi* Factory::CreateGapiGL() 
{
#ifdef BUILD_DLL
	return ((IGapi*(*)())Sys::GetDLLProcAddress(Sys::LoadDLL((Sys::GetWorkDir() + L"GraphicsApiGL").c_str()), "createGraphicsApi"))();
#elif BUILD_STATIC
	return (IGapi*)new GapiGL();
#endif
}

IWindow* Factory::CreateWindow(const rWindow& d)
{
	return new Window(d);
}

ITimer* Factory::CreateTimer()
{
	return new Timer();
}