#include "Factory.h"
#include "Sys.h"


#ifdef	WIN
	#include "..\CommonWin\src\Window.h"
#elif	LIN
	#include "..\CommonLin\src\Window.h"
#elif	MAC
	#include "..\CommonMac\src\Window.h"
#endif

ge::IGraphicsEngine* Factory::createGraphicsEngineRaster(const ge::IGraphicsEngine::rCfg& d) {
	return ((ge::IGraphicsEngine*(*)())Sys::getDllProcAddress(Sys::loadDLL("GraphicsEngineRaster"), "CreateGraphicsEngine"))();
}

ge::IGraphicsEngine* Factory::createGraphicsEngineRT(const ge::IGraphicsEngine::rCfg& d) {
	return nullptr;
}

IWindow* Factory::createWindow(const IWindow::rDesc& d) {
	return new Window(d);
}