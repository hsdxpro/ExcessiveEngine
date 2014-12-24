#pragma once
#include "../GraphicsEngine_Interface/interface/IGraphicsEngine.h"

#include <unordered_set>

// must be included because of covariant return type override
#include "../GraphicsEngine_Interface/interface/IScene.h"
#include "../GraphicsEngine_Interface/interface/IMaterial.h"
#include "../GraphicsEngine_Interface/interface/IMesh.h"
#include "../GraphicsEngineRaster/src/Camera.h"


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

	// interact
	void update() override;

private:
};

