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
	#include "..\GraphicsApiGL\src\Gapi.h"
#endif

ge::IGraphicsEngine* Factory::createGraphicsEngineRaster(const ge::rGraphicsEngine& d) {
#ifdef BUILD_DLL	
	return ((ge::IGraphicsEngine*(*)())Sys::getDllProcAddress(Sys::loadDLL("GraphicsEngineRaster"), "CreateGraphicsEngine"))();
#elif BUILD_STATIC	
	return new ge::GraphicsEngineRaster(d);
#endif
}

ge::IGapi* Factory::createGapiGL() {
#ifdef BUILD_DLL	
	return ((ge::IGapi*(*)())Sys::getDllProcAddress(Sys::loadDLL("GraphicsApiGL"), "createGraphicsApi"))();
#elif BUILD_STATIC	
	return (ge::IGapi*)new GapiGL();
#endif
}

IWindow* Factory::createWindow(const rWindow& d) {
	return new Window(d);
}