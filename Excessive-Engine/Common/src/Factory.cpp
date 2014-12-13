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
	#include "..\GraphicsApiGL\src\GapiGL.h"
#endif

#include "..\GraphicsEngineRaster\src\ResourceLoader.h"

graphics::IGraphicsEngine* Factory::createGraphicsEngineRaster(const graphics::rGraphicsEngine& d) {
#ifdef BUILD_DLL
	using CreateT = graphics::IGraphicsEngine*(*)(const graphics::rGraphicsEngine& d);
	auto module = Sys::loadDLL("GraphicsEngineRaster");
	if (!module) {
		return nullptr;
	}
	CreateT CreateGraphicsEngine = (CreateT)Sys::getDllProcAddress(module, "CreateGraphicsEngine");
	if (!CreateGraphicsEngine) {
		return nullptr;
	}
	return CreateGraphicsEngine(d);
#elif BUILD_STATIC	
	return new GraphicsEngineRaster(d);
#endif
}

graphics::IResourceLoader* Factory::createResourceLoader() {
	return (graphics::IResourceLoader*)new ResourceLoader;
}
IGapi* Factory::createGapiGL() {
#ifdef BUILD_DLL
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
	return ((IGapi*(*)())Sys::getDllProcAddress(Sys::loadDLL(converter.to_bytes(Sys::getWorkDir()) + "GraphicsApiGL"), "createGraphicsApi"))();
#elif BUILD_STATIC
	return (IGapi*)new GapiGL();
#endif
}

IWindow* Factory::createWindow(const rWindow& d) {
	return new Window(d);
}