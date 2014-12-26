// IWindow, IGraphicsEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once

// Graphics Engine
#include "../GraphicsEngineRT/src/GraphicsEngine.h"
#include "../GraphicsEngineRaster/src/GraphicsEngine.h"
#include "../GraphicsEngine_Interface/interface/IGraphicsEngine.h"

#include "../GraphicsApi_Interface/interface/IGapi.h"
#include "IWindow.h"

class Factory
{
public:
	static graphics::IGraphicsEngine*	createGraphicsEngineRaster(const rGraphicsEngineRaster& d);
	static graphics::IGraphicsEngine*	createGraphicsEngineRT(const rGraphicsEngineRT& d);
	static IWindow*						createWindow(const struct rWindow& d);
	static IGapi*						createGapiGL();
};