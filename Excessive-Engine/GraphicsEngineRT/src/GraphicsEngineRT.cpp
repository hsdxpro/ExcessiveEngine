#include "GraphicsEngineRT.h"


////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C"
EXPORT graphics::IGraphicsEngine* CreateGraphicsEngineRT(const rGraphicsEngineRT& d) {
	return new GraphicsEngineRT(d);
}


////////////////////////////////////////////////////////////////////////////////
// GraphicsEngine class
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

GraphicsEngineRT::GraphicsEngineRT(const rGraphicsEngineRT& d) {
}

GraphicsEngineRT::~GraphicsEngineRT() {
}


void GraphicsEngineRT::release() {
	delete this;
}


////////////////////////////////////////////////////////////////////////////////
// create stuff
graphics::IScene* GraphicsEngineRT::createScene() {
	return 0;
}

void GraphicsEngineRT::deleteScene(graphics::IScene* scene) {
}

graphics::IMesh* GraphicsEngineRT::createMesh() {
	return nullptr;
}

graphics::IMaterial* GraphicsEngineRT::createMaterial() {
	return nullptr;
}

graphics::ITexture* GraphicsEngineRT::createTexture() {
	return nullptr;
}

Camera* GraphicsEngineRT::createCam() {
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// update
void GraphicsEngineRT::update(float deltaTime) {
	
}