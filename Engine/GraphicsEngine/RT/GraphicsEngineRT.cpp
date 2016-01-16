#include "GraphicsEngineRT.h"

GraphicsEngineRT_Richard::GraphicsEngineRT_Richard(const rGraphicsEngineRT_Richard& d)
:targetWindow(d.targetWindow)
{
	
}

GraphicsEngineRT_Richard::~GraphicsEngineRT_Richard() 
{

}

void GraphicsEngineRT_Richard::Release() 
{
	delete this;
}

graphics::IScene* GraphicsEngineRT_Richard::CreateScene() {
	return 0;
}

graphics::IMesh* GraphicsEngineRT_Richard::CreateMesh() 
{
	return nullptr;
}

graphics::IMaterial* GraphicsEngineRT_Richard::CreateMaterial() 
{
	return nullptr;
}

graphics::ITexture* GraphicsEngineRT_Richard::CreateTexture() 
{
	return nullptr;
}

Camera* GraphicsEngineRT_Richard::CreateCam() 
{
	return nullptr;
}

void GraphicsEngineRT_Richard::Update(float deltaTime) 
{

}

void GraphicsEngineRT_Richard::AddLayer(const Layer& layer) 
{

}

void GraphicsEngineRT_Richard::RemoveLayer(size_t index) 
{

}

size_t GraphicsEngineRT_Richard::GetNumLayers() const 
{
	return 0;
}

void GraphicsEngineRT_Richard::SetNumLayers(size_t num_layers) 
{

}

auto GraphicsEngineRT_Richard::GetLayer(size_t index) -> Layer& 
{
	static Layer l;
	return l;
}