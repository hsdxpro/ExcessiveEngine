// IWindow, IGraphicsEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once

#include "..\GraphicsEngine_Interface\interface\IGraphicsEngine.h"
#include "IWindow.h"

class Factory
{
public:
	static ge::IGraphicsEngine* createGraphicsEngine(const ge::IGraphicsEngine::rDesc& d);
	static IWindow* createWindow(const IWindow::rDesc& d);
};
