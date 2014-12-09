#include "GraphicsEngine.h"

#include <iostream> // only for debug
using std::cout;
using std::endl;


#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif


////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

extern "C"
EXPORT ge::IGraphicsEngine* CreateGraphicsEngine(const ge::IGraphicsEngine::rCfg& d) {
	return new ge::GraphicsEngineRaster(d);
}


////////////////////////////////////////////////////////////////////////////////
// GraphicsEngine class
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

ge::GraphicsEngineRaster::GraphicsEngineRaster(const rCfg& d) {
	// lóóóófasz rizzsel
}

ge::GraphicsEngineRaster::~GraphicsEngineRaster() {
	// lófasz rizzsel
}


void ge::GraphicsEngineRaster::release() {
	delete this;
}


////////////////////////////////////////////////////////////////////////////////
// create stuff
Scene* ge::GraphicsEngineRaster::createScene() {
	return new Scene;
}

Mesh* ge::GraphicsEngineRaster::createMesh() {
	return new Mesh(nullptr);
}

Material* ge::GraphicsEngineRaster::createMaterial() {
	return new Material;
}

Texture* ge::GraphicsEngineRaster::createTexture() {
	return new Texture;
}


////////////////////////////////////////////////////////////////////////////////
// update
void ge::GraphicsEngineRaster::update() {
	cout << "Updating frame..." << endl;
}

ge::IGapi* ge::GraphicsEngineRaster::getGapi() {
	return nullptr;
}