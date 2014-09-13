#include "GraphicsEngine.h"




GraphicsEngine::GraphicsEngine() {
	// lóóóófasz rizzsel
}

GraphicsEngine::~GraphicsEngine() {
	// lófasz rizzsel
}


void GraphicsEngine::release() {
	delete this;
}


ge::IScene* GraphicsEngine::createScene() {
	return nullptr;
}