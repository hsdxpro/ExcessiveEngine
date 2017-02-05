#pragma once
#include "BaseLibrary\Types.h"
#include "PlatformLibrary\Window.h"

class IGapi;
class ICamera;

namespace graphics {
	class IScene;
	class IMesh;
	class IMaterial;
	class ITexture;
}

////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine manages and displays the graphical world.
/// No descrption yet.
////////////////////////////////////////////////////////////////////////////////

class IGraphicsEngine
{
public:
	/// Layer properties.
	/// Layers works much like photoshop's layers, except they are rendered in real-time
	/// from graphical scenes.
	struct Layer 
	{
		enum eBlendMode : u32 
		{
			OVERLAY, ///< Draw this layer simply onto the layer below.
			MULTIPLY, ///< multiply colors componentwise.
			ADD, ///< Add colors componentswise.
			SUBTRACT, ///< subtract this from the layer below.
			MERGE, ///< merge contents with layer below.
		};

		Layer() : blendMode(OVERLAY), scene(nullptr), opacity(1.0f), visible(true), preserve_depth(false) {}

		graphics::IScene* scene; ///< this scene yields the image for this layer.
		float opacity;
		bool visible; ///< weather to render and Add the layer
		bool preserve_depth; ///< re-uses the below layer's depth map for rendering.
		eBlendMode blendMode;
	};

	/// Release the interface.
	/// Don't forget to call this when this object is no longer needed.
	virtual void Release() = 0;


// Create stuff

	/// Create a new empty scene.
	/// You may Add objects and other things to the scene.
	/// The scene can be rendered by the engine. To Get it rendered, Set it in a Layer.
	virtual graphics::IScene* CreateScene() = 0;

	/// Create a new mesh resource.
	virtual graphics::IMesh* CreateMesh() = 0;

	/// Create new material resource.
	virtual graphics::IMaterial* CreateMaterial() = 0;

	/// Create new texture resource.
	virtual graphics::ITexture* CreateTexture() = 0;

	/// Create new virtual camera.
	virtual ICamera* CreateCam() = 0;

	virtual void Update(float deltaTime) = 0;

	/// Resize Render Target textures
	virtual bool ResizeRenderTargets(unsigned width, unsigned height) = 0;

// scene & layer system

	virtual void AddLayer(const Layer& layer) = 0;
	virtual void RemoveLayer(size_t index) = 0;
	virtual size_t GetNumLayers() const = 0;
	virtual void SetNumLayers(size_t num_layers) = 0;
	virtual Layer& GetLayer(size_t index) = 0;

	virtual Window* GetTargetWindow() = 0;
};