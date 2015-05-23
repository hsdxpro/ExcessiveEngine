#pragma once
#include "../../Interfaces/IEngine.h"

// must be included because of covariant return type override
#include "../../Interfaces/IScene.h"
#include "../../Interfaces/IMaterial.h"
#include "../../Interfaces/IMesh.h"
#include "../../Raster/src/Camera.h"

#include <unordered_set>

////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine implementation.
///
////////////////////////////////////////////////////////////////////////////////

struct rGraphicsEngineRT {

};

class GraphicsEngineRT : public graphics::IEngine
{
public:
	// ctor, dtor, release
	GraphicsEngineRT(const rGraphicsEngineRT& d);
	~GraphicsEngineRT();

	void release() override;

	// create stuff
	graphics::IScene*	createScene() override;
	graphics::IMesh*	 createMesh() override;
	graphics::IMaterial* createMaterial() override;
	graphics::ITexture*  createTexture() override;
	Camera* createCam() override;

	void addLayer(const Layer& layer) override;
	void removeLayer(size_t index) override;
	size_t getNumLayers() const override;
	void setNumLayers(size_t num_layers) override;
	Layer& getLayer(size_t index) override;

	// state
	void setResolution(u32 width, u32 height) override {}

	// interact
	void update(float deltaTime) override;

	IGapi* getGapi() override { return nullptr;  }

private:
};

