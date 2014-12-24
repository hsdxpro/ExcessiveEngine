#include "Factory.h"
#include <locale>
#include <codecvt>

#ifdef	WIN
	#include "..\CommonWin\src\Window.h"
#elif	LIN
	#include "..\CommonLin\src\Window.h"
#elif	MAC
	#include "..\CommonMac\src\Window.h"
#endif

#ifdef BUILD_STATIC
	#include "..\GraphicsEngineRaster\src\GraphicsEngine.h"
	#include "..\GraphicsEngineRT\src\GraphicsEngine.h"
	#include "..\GraphicsApiGL\src\GapiGL.h"
#endif

#include "ResourceLoader.h"

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
	CreateT CreateGraphicsEngine = (CreateT)Sys::getDllProcAddress(module, "CreateGraphicsEngine");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#elif BUILD_STATIC	
	return new GraphicsEngineRT(d);
#endif
}

graphics::IResourceLoader* Factory::createResourceLoader() {
	return (graphics::IResourceLoader*)new ResourceLoader;
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