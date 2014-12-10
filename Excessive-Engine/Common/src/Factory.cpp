#include "Factory.h"

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

ge::IGraphicsEngine* Factory::createGraphicsEngineRaster(const ge::rGraphicsEngine& d) {
#ifdef BUILD_DLL	
	return ((ge::IGraphicsEngine*(*)())Sys::getDllProcAddress(Sys::loadDLL("GraphicsEngineRaster"), "CreateGraphicsEngine"))();
#elif BUILD_STATIC	
	return new ge::GraphicsEngineRaster(d);
#endif
}

IGapi* Factory::createGapiGL() {
#ifdef BUILD_DLL	
	return ((IGapi*(*)())Sys::getDllProcAddress(Sys::loadDLL(Sys::getWorkDir() + "GraphicsApiGL"), "createGraphicsApi"))();
#elif BUILD_STATIC	
	return (IGapi*)new GapiGL();
#endif
}

IWindow* Factory::createWindow(const rWindow& d) {
	return new Window(d);
}