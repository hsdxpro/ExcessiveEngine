#pragma once

#include "../../GraphicsApi_Interface/interface/IGapi.h"

// namespace
namespace graphics {

class IScene;
class IMesh;
class IMaterial;
class ITexture;
class ICamera;

//struct rGraphicsEngine {
//	IGapi* gapi;
//};


////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine manages and displays the graphical world.
/// No descrption yet.
////////////////////////////////////////////////////////////////////////////////

class IGraphicsEngine
{
public:
	// interface
	virtual void release() = 0;

	// create stuff
	virtual IScene* createScene() = 0;
	virtual void deleteScene(IScene* scene) = 0;
	virtual IMesh* createMesh() = 0;
	virtual IMaterial* createMaterial() = 0;
	virtual ITexture* createTexture() = 0;
	virtual ICamera* createCam() = 0;

	// states | just as examples, not stable yet
		// virtual void setTargetWindow();
	virtual void setResolution(u32 width, u32 height) = 0;

	// interact
	virtual void update() = 0;
};


} // namespace