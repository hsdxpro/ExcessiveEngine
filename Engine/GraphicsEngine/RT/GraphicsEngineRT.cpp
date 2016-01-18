#include "GraphicsEngineRT.h"
#include "..\Raster\Scene.h"
#include "Core\EngineCore.h"

GraphicsEngineRT_Richard::GraphicsEngineRT_Richard(const rGraphicsEngineRT_Richard& d)
	:targetWindow(d.targetWindow)
{
	backBuffer = new Color[targetWindow->GetNumClientPixels()];
}

GraphicsEngineRT_Richard::~GraphicsEngineRT_Richard() 
{
	delete backBuffer;
}

void GraphicsEngineRT_Richard::Release() 
{
	delete this;
}

graphics::IScene* GraphicsEngineRT_Richard::CreateScene() 
{
	return new Scene();
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
	std::vector<mm::vec3> vertices = Core.loadedPhysicalVertexPositions;

	// Need view, proj matrix
	// Transform each vertex into screen space
	// Then rasterize it

	// Run return piros kernel
	// Read result into backbuffer

	// Copy result to backbuffer
	targetWindow->SetClientPixels(backBuffer);
}

void GraphicsEngineRT_Richard::AddLayer(const Layer& layer) 
{
	sceneLayers.push_back(layer);
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