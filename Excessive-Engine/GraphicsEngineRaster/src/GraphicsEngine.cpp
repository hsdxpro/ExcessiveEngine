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
	return new GraphicsEngine(d);
}


////////////////////////////////////////////////////////////////////////////////
// GraphicsEngine class
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

GraphicsEngine::GraphicsEngine(const rCfg& d) {
	// lóóóófasz rizzsel
}

GraphicsEngine::~GraphicsEngine() {
	// lófasz rizzsel
}


void GraphicsEngine::release() {
	delete this;
}


////////////////////////////////////////////////////////////////////////////////
// create stuff
Scene* GraphicsEngine::createScene() {
	return new Scene;
}

Mesh* GraphicsEngine::createMesh() {
	return new Mesh;
}

Material* GraphicsEngine::createMaterial() {
	return new Material;
}

Texture* GraphicsEngine::createTexture() {
	return new Texture;
}


////////////////////////////////////////////////////////////////////////////////
// update
void GraphicsEngine::update() {
	cout << "Updating frame..." << endl;
}

ge::IGapi* GraphicsEngine::getGapi() {
	return nullptr;
}