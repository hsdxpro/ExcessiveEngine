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
EXPORT graphics::IEngine* CreateGraphicsEngineRT(const rGraphicsEngineRT& d) {
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





void GraphicsEngineRT::addLayer(const Layer& layer) {
}

void GraphicsEngineRT::removeLayer(size_t index) {
}

size_t GraphicsEngineRT::getNumLayers() const {
	return 0;
}

void GraphicsEngineRT::setNumLayers(size_t num_layers) {
}

auto GraphicsEngineRT::getLayer(size_t index) -> Layer& {
	static Layer l;
	return l;
}