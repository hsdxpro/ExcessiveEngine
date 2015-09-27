#pragma once
#include "../IGraphicsEngine.h"

// must be included because of covariant return type override
#include "../IScene.h"
#include "../IMaterial.h"
#include "../IMesh.h"
#include "../Raster/Camera.h"

#include <unordered_set>
#include "PlatformLibrary/Window.h"



////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine implementation.
///
////////////////////////////////////////////////////////////////////////////////

struct rGraphicsEngineRT {

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

	IGapi* GetGapi() override			{return nullptr;}
	Window* GetTargetWindow() override {return nullptr;}
};

