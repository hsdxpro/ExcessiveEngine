#include "Factory.h"
#include <locale>
#include <codecvt>

#include <GraphicsApi/OpenGL/GapiGL.h>
#include "PlatformLibrary/Sys.h"


IGraphicsEngine* Factory::CreateGraphicsEngineRaster(const rGraphicsEngineRasterData& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = IGraphicsEngine*(*)(const rGraphicsEngineRasterData& d);
	auto module = Sys::LoadDLL(L"GraphicsEngineRaster");
	if (!module) {
		return nullptr;
	}
	CreateT CreateGraphicsEngine = (CreateT)Sys::GetDLLProcAddress(module, "CreateGraphicsEngineRaster");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#else	
	return new GraphicsEngineRaster(d);
#endif
}

IGraphicsEngine* Factory::CreateGraphicsEngineRT(const rGraphicsEngineRT& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = IGraphicsEngine*(*)(const rGraphicsEngineRT& d);
	auto module = Sys::LoadDLL(L"GraphicsEngineRasterRT");
	if (!module) {
		return nullptr;
	}
	CreateT CreateGraphicsEngine = (CreateT)Sys::GetDLLProcAddress(module, "CreateGraphicsEngineRT");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#else
	return new GraphicsEngineRT(d);
#endif
}

IPhysicsEngine* Factory::CreatePhysicsEngineBullet(const rPhysicsEngineBullet& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = IPhysicsEngine*(*)(const rPhysicsEngineBullet& d);
	auto module = Sys::LoadDLL(L"PhysicsEngineBullet");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::GetDLLProcAddress(module, "CreatePhysicsEngineBullet");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#else
	return new PhysicsEngineBullet(d);
#endif
}

INetworkEngine* Factory::CreateNetworkEngine(const rNetworkEngine& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = INetworkEngine*(*)(const rNetworkEngine& d);
	auto module = Sys::LoadDLL(L"NetworkEngineBoost");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::GetDLLProcAddress(module, "CreateNetworkEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#else
	return new NetworkEngineBoost(d);
#endif
}

ISoundEngine* Factory::CreateSoundEngine(const rSoundEngine& d) 
{
#ifdef EXCESSIVE_DLL_MODULES
	using CreateT = ISoundEngine*(*)(const rSoundEngine& d);
	auto module = Sys::LoadDLL(L"SoundEngineSFML");
	if (!module) {
		return nullptr;
	}
	CreateT creator = (CreateT)Sys::GetDLLProcAddress(module, "CreateSoundEngine");
	if (!creator) {
		return nullptr;
	}
	return creator(d);
#else
	return new SoundEngineSFML(d);
#endif
}

IGapi* Factory::CreateGapiGL() 
{
#ifdef EXCESSIVE_DLL_MODULES
	return ((IGapi*(*)())Sys::GetDLLProcAddress(Sys::LoadDLL(L"GraphicsApiGL"), "CreateGraphicsApi"))();
#else
	return (IGapi*)new GapiGL();
#endif
}