#include "Factory.h"
#include "Sys.h"

#ifdef	WIN
	#include "..\CommonWin\src\Window.h"
#elif	LIN
	#include "..\CommonLin\src\Window.h"
#elif	MAC
	#include "..\CommonMac\src\Window.h"
#endif

#ifdef BUILD_STATIC
	#include "..\GraphicsEngineRaster\src\GraphicsEngine.h"
	//#include "..\GraphicsEngineRTr\src\GraphicsEngine.h"
#endif


ge::IGraphicsEngine* Factory::createGraphicsEngineRaster(const ge::IGraphicsEngine::rCfg& d) {
#ifdef BUILD_DLL	
	return ((ge::IGraphicsEngine*(*)())Sys::getDllProcAddress(Sys::loadDLL("GraphicsEngineRaster"), "CreateGraphicsEngine"))();
#elif BUILD_STATIC	
	return new ge::GraphicsEngineRaster(d);
#endif
}

ge::IGraphicsEngine* Factory::createGraphicsEngineRT(const ge::IGraphicsEngine::rCfg& d) {
#ifdef BUILD_DLL
	return ((ge::IGraphicsEngine*(*)())Sys::getDllProcAddress(Sys::loadDLL("GraphicsEngineRT"), "CreateGraphicsEngine"))();
#elif BUILD_STATIC
	return nullptr;
#endif
}

IWindow* Factory::createWindow(const IWindow::rDesc& d) {
	return new Window(d);
}