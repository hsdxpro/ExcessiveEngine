#pragma once

#include "../../Interfaces/IEngine.h"


// must be included because of covariant return type override
#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Camera.h"
#include "../../../GraphicsApi/Interfaces/IGapi.h"

#include <unordered_set>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine implementation.
///
////////////////////////////////////////////////////////////////////////////////

enum class eGapiType {
	OPENGL_4_5,
};

struct rGraphicsEngineRaster {
	eGapiType type;
};

class GraphicsEngineRaster : public graphics::IEngine
{
public:
	// ctor, dtor, release
	GraphicsEngineRaster(const rGraphicsEngineRaster& d);
	~GraphicsEngineRaster();
	bool isConstructionSucceeded() const { return isValid; }

	void release() override;

	// create stuff
	Scene* createScene() override;
	Mesh* createMesh() override;
	Material* createMaterial() override;
	Texture* createTexture() override;
	Camera* createCam() override;
	
	// scene & layer system
	void addLayer(const Layer& layer) override;
	void removeLayer(size_t index) override;
	size_t getNumLayers() const override;
	void setNumLayers(size_t num_layers) override;
	Layer& getLayer(size_t index) override;

	// states
	void setResolution(u32 width, u32 height) override;

	// interact
	void update(float deltaTime) override;

	IGapi* getGapi();

private:
	IGapi* gapi;
	std::vector<Layer> layers;

	// WARNING: temporary code
	IShaderProgram* shader;

	// WARNING: pretty crappy solution
	bool isValid;
};

