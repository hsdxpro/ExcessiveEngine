// IWindow, IGraphicsEngine, etc interfacek mögött levő példányok létrehozására használatos
#pragma once

// Predecl
namespace ge { class IGraphicsEngine; class IGapi; struct rGraphicsEngine; }

class Factory
{
public:
	static ge::IGraphicsEngine*	createGraphicsEngineRaster(const ge::rGraphicsEngine& d);
	static ge::IGapi*			createGapiGL();
	static class IWindow*		createWindow(const struct rWindow& d);
};