#pragma once
#include "GraphicsEngine/IGraphicsEngine.h"

// must be included because of covariant return type override
#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Camera.h"
#include "GraphicsApi/IGapi.h"

#include <unordered_set>
#include <vector>
#include "GraphicsApi/IGapi.h"

class IShaderProgram;

struct rRasterGraphicsEngine
{
	Window*		targetWindow;
	eGapiType	gapiType;
	rRectNormed	renderRegion;
};

class RasterGraphicsEngine : public IGraphicsEngine
{
public:
	RasterGraphicsEngine(const rRasterGraphicsEngine& d);
	~RasterGraphicsEngine();
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

