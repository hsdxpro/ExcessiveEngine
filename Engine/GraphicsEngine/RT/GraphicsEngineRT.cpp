#include "GraphicsEngineRT.h"
#include "Core\EngineCore.h"

GraphicsEngineRT::GraphicsEngineRT(const rGraphicsEngineRT& d)
	:targetWindow(d.targetWindow)
{
	backBuffer = new Color[targetWindow->GetNumClientPixels()];
}

GraphicsEngineRT::~GraphicsEngineRT()
{
	delete backBuffer;
}

void GraphicsEngineRT::Release()
{
	delete this;
}

graphics::IScene* GraphicsEngineRT::CreateScene()
{
	return new Scene();
}

graphics::IMesh* GraphicsEngineRT::CreateMesh()
{
	return nullptr;
}

graphics::IMaterial* GraphicsEngineRT::CreateMaterial()
{
	return nullptr;
}

graphics::ITexture* GraphicsEngineRT::CreateTexture()
{
	return nullptr;
}

Camera* GraphicsEngineRT::CreateCam()
{
	return nullptr;
}

void GraphicsEngineRT::Update(float deltaTime)
{
	// Need view, proj matrix
	// Transform each vertex into screen space
	// Then rasterize it

	// Run return piros kernel
	// Read result into backbuffer

	// Copy result to backbuffer
	targetWindow->SetClientPixels(backBuffer);
}

void GraphicsEngineRT::AddLayer(const Layer& layer)
{
	sceneLayers.push_back(layer);
}

void GraphicsEngineRT::RemoveLayer(size_t index)
{

}

size_t GraphicsEngineRT::GetNumLayers() const
{
	return 0;
}

void GraphicsEngineRT::SetNumLayers(size_t num_layers)
{

}

auto GraphicsEngineRT::GetLayer(size_t index) -> Layer&
{
	static Layer l;
	return l;
}