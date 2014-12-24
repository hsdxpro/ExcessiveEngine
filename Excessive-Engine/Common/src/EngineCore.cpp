#include "EngineCore.h"
#include "Factory.h"

EngineCore::EngineCore() 
:graphicsEngine(0) {
}

graphics::IGraphicsEngine* EngineCore::initGraphicsEngineRaster(const rGraphicsEngineRaster& desc) {
	if (graphicsEngine) 
		graphicsEngine->release(); 

	return Factory::createGraphicsEngineRaster(desc);
}

// Init graphics engine, if one already exists will be destroyed, then instantiate it
graphics::IGraphicsEngine* EngineCore::initGraphicsEngineRT(const rGraphicsEngineRT& desc) {
	if (graphicsEngine) 
		graphicsEngine->release(); 
	
	return Factory::createGraphicsEngineRT(desc);
}