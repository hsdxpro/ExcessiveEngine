#pragma once

#include "../IGraphicsEngine.h"


// must be included because of covariant return type override
#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Camera.h"
#include <GraphicsApi/IGapi.h>

#include <unordered_set>
#include <vector>


class IWindow;
class IShaderProgram;

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
	Window*			targetWindow;
	rRectNormed		renderRegion;
};

struct rGraphicsEngineRasterData
{
	IGapi*			gapi;
	Window*		targetWindow;
	rRectNormed		renderRegion;
};

class GraphicsEngineRaster : public IGraphicsEngine
{
public:
	// ctor, dtor, release
	GraphicsEngineRaster(const rGraphicsEngineRasterData& d);
	~GraphicsEngineRaster();
	bool isConstructionSucceeded() const { return isValid; }

	void Release() override;

	// Create stuff
	Scene* CreateScene() override;
	Mesh* CreateMesh() override;
	Material* CreateMaterial() override;
	Texture* CreateTexture() override;
	Camera* CreateCam() override;
	
	// scene & layer system
	// DEPRECATED
	void AddLayer(const Layer& layer) override;
	void RemoveLayer(size_t index) override;
	size_t GetNumLayers() const override;
	void SetNumLayers(size_t num_layers) override;
	Layer& GetLayer(size_t index) override;

	// configure pipeline
	// description must be in json format.
	// describes the nodes making the pipeline
	bool SetPipeline(const char* description);

	// interact
	void Update(float deltaTime) override;

	IGapi* GetGapi() override;

	Window* GetTargetWindow() override;
private:
	IGapi* gapi;
	std::vector<Layer> layers;

	Window* targetWindow;
	rRectNormed	renderRegion;

	// WARNING: temporary code
	IShaderProgram* shader;

	// WARNING: pretty crappy solution
	bool isValid;
};

