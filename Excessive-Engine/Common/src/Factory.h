// IWindow, IGraphicsEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once

#include "IWindow.h"
#include "..\GraphicsEngine_Interface\interface\IGraphicsEngine.h"

class Factory
{
public:
	static ge::IGraphicsEngine* createGraphicsEngineRaster(const ge::IGraphicsEngine::rCfg& d);
	static ge::IGraphicsEngine* createGraphicsEngineRT(const ge::IGraphicsEngine::rCfg& d);
	static IWindow*				createWindow(const IWindow::rDesc& d);
};