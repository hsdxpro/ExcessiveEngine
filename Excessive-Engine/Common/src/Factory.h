// IWindow, IGraphicsEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once

namespace ge{ class IGraphicsEngine;};
class IWindow;

class Factory
{
public:
	static ge::IGraphicsEngine* createGraphicsEngineRaster();
	static ge::IGraphicsEngine* createGraphicsEngineRT();
	static IWindow* createWindow();
};
