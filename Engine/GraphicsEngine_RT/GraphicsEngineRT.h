#pragma once

// must be included because of covariant return type override
#include "GraphicsEngine/IGraphicsEngine.h"
#include "GraphicsEngine/IScene.h"
#include "GraphicsEngine/IMaterial.h"
#include "GraphicsEngine/IMesh.h"
#include "SupportLibrary/Camera.h"

struct rGraphicsEngineRT
{
	Window* targetWindow;
};

class GraphicsEngineRT : public IGraphicsEngine
{
public:
	// ctor, dtor, release
	GraphicsEngineRT(const rGraphicsEngineRT& d);
	~GraphicsEngineRT();

	void Release() override;

	// Create stuff
	graphics::IScene*	CreateScene() override;
	graphics::IMesh*	 CreateMesh() override;
	graphics::IMaterial* CreateMaterial() override;
	graphics::ITexture*  CreateTexture() override;
	Camera* CreateCam() override;

	void AddLayer(const Layer& layer) override;
	void RemoveLayer(size_t index) override;
	size_t GetNumLayers() const override;
	void SetNumLayers(size_t num_layers) override;
	Layer& GetLayer(size_t index) override;

	// interact
	void Update(float deltaTime) override;

	Window* GetTargetWindow() override {return targetWindow;}

protected:
	Window* targetWindow;

	Color* backBuffer;
	std::vector<Layer> sceneLayers;
};