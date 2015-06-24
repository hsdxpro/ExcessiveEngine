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


void GraphicsEngineRT::Release() {
	delete this;
}


////////////////////////////////////////////////////////////////////////////////
// Create stuff
graphics::IScene* GraphicsEngineRT::CreateScene() {
	return 0;
}

graphics::IMesh* GraphicsEngineRT::CreateMesh() {
	return nullptr;
}

graphics::IMaterial* GraphicsEngineRT::CreateMaterial() {
	return nullptr;
}

graphics::ITexture* GraphicsEngineRT::CreateTexture() {
	return nullptr;
}

Camera* GraphicsEngineRT::CreateCam() {
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// Update
void GraphicsEngineRT::Update(float deltaTime) {
	
}





void GraphicsEngineRT::AddLayer(const Layer& layer) {
}

void GraphicsEngineRT::RemoveLayer(size_t index) {
}

size_t GraphicsEngineRT::GetNumLayers() const {
	return 0;
}

void GraphicsEngineRT::SetNumLayers(size_t num_layers) {
}

auto GraphicsEngineRT::GetLayer(size_t index) -> Layer& {
	static Layer l;
	return l;
}