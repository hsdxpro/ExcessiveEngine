// IWindow, IGraphicsEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once
#include "..\GraphicsEngine_Interface\interface\IGraphicsEngine.h"
#include "..\GraphicsApi_Interface\interface\IGapi.h"
#include "IWindow.h"

class Factory
{
public:
	static graphics::IGraphicsEngine*	createGraphicsEngineRaster(const graphics::rGraphicsEngine& d);
	static IGapi*				createGapiGL();
	static class IWindow*		createWindow(const struct rWindow& d);
};