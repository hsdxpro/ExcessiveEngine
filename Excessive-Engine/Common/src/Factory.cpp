#include "Factory.h"
#include "Sys.h"

ge::IGraphicsEngine* Factory::createGraphicsEngineRaster() {
	return ((ge::IGraphicsEngine*(*)())Sys::getDllProcAddress(Sys::loadDLL("GraphicsEngineRaster"), "CreateGraphicsEngine"))();
}

ge::IGraphicsEngine* Factory::createGraphicsEngineRT() {
	return nullptr;
}

IWindow* Factory::createWindow() {
	return nullptr;
}