#include "GraphicsEngine.h"

#include <iostream> // only for debug
#include "..\Common\src\Factory.h"
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
EXPORT ge::IGraphicsEngine* CreateGraphicsEngine(const ge::rGraphicsEngine& d) {
	return new ge::GraphicsEngineRaster(d);
}


////////////////////////////////////////////////////////////////////////////////
// GraphicsEngine class
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

ge::GraphicsEngineRaster::GraphicsEngineRaster(const rGraphicsEngine& d) {
	gapi = d.gapi;
}

ge::GraphicsEngineRaster::~GraphicsEngineRaster() {
	// TODO
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
	return new Mesh;
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

IGapi* ge::GraphicsEngineRaster::getGapi() {
	return gapi;
}