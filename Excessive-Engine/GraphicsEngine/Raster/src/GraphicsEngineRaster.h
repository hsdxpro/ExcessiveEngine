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

class IWindow;

////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine implementation.
///
////////////////////////////////////////////////////////////////////////////////

enum class eGapiType 
{
	OPENGL_4_5,
};

// TODO move to better place
struct rRectNormed
{
	rRectNormed() :bottomLeftPercentNormed(0, 0), topRightPercentNormed(1, 1){}

	mm::vec2 bottomLeftPercentNormed;
	mm::vec2 topRightPercentNormed;
};

struct rGraphicsEngineRaster 
{
	eGapiType		gapiType;
	IWindow*		targetWindow;
	rRectNormed		renderRegion;
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

	// interact
	void update(float deltaTime) override;

	IGapi* getGapi() override;

private:
	IGapi* gapi;
	std::vector<Layer> layers;

	// ? TMP
	IWindow* targetWindow;
	rRectNormed	renderRegion;

	// WARNING: temporary code
	IShaderProgram* shader;

	// WARNING: pretty crappy solution
	bool isValid;
};

