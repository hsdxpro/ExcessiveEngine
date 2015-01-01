#pragma once
#include "../../Module_Interfaces/GraphicsEngine/IGraphicsEngine.h"

// must be included because of covariant return type override
#include "../Module_Interfaces/GraphicsEngine/IScene.h"
#include "../Module_Interfaces/GraphicsEngine/IMaterial.h"
#include "../Module_Interfaces/GraphicsEngine/IMesh.h"
#include "../GraphicsEngineRaster/src/Camera.h"

#include <unordered_set>

////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine implementation.
///
////////////////////////////////////////////////////////////////////////////////

struct rGraphicsEngineRT {

};

class GraphicsEngineRT : public graphics::IGraphicsEngine
{
public:
	// ctor, dtor, release
	GraphicsEngineRT(const rGraphicsEngineRT& d);
	~GraphicsEngineRT();

	void release() override;

	// create stuff
	graphics::IScene*	createScene() override;
	void				deleteScene(graphics::IScene* scene) override;

	graphics::IMesh*	 createMesh() override;
	graphics::IMaterial* createMaterial() override;
	graphics::ITexture*  createTexture() override;

	Camera* createCam() override;

	// state
	void setResolution(u32 width, u32 height) override {}

	// interact
	void update(float deltaTime) override;

private:
};

