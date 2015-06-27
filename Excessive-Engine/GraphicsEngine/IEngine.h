#pragma once
#include "../Common/src/BasicTypes.h"

class IGapi;

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

class IEngine
{
public:
	/// Layer properties.
	/// Layers works much like photoshop's layers, except they are rendered in real-time
	/// from graphical scenes.
	struct Layer {
		enum eBlendMode : u32 {
			OVERLAY, ///< Draw this layer simply onto the layer below.
			MULTIPLY, ///< multiply colors componentwise.
			ADD, ///< Add colors componentswise.
			SUBTRACT, ///< subtract this from the layer below.
			MERGE, ///< merge contents with layer below.
		};

		Layer() : blendMode(OVERLAY), scene(nullptr), opacity(1.0f), visible(true), preserve_depth(false) {}

		IScene* scene; ///< this scene yields the image for this layer.
		float opacity;
		bool visible; ///< weather to render and Add the layer
		bool preserve_depth; ///< re-uses the below layer's depth map for rendering.
		eBlendMode blendMode;
	};


	/// Release the interface.
	/// Don't forget to call this when this object is no longer needed.
	virtual void Release() = 0;


	///////////////////////////////////////
	// Create stuff

	/// Create a new empty scene.
	/// You may Add objects and other things to the scene.
	/// The scene can be rendered by the engine. To Get it rendered, Set it in a Layer.
	virtual IScene* CreateScene() = 0;

	/// Create a new mesh resource.
	virtual IMesh* CreateMesh() = 0;

	/// Create new material resource.
	virtual IMaterial* CreateMaterial() = 0;

	/// Create new texture resource.
	virtual ITexture* CreateTexture() = 0;

	/// Create new virtual camera.
	virtual ICamera* CreateCam() = 0;


	///////////////////////////////////////
	// scene & layer system

	virtual void AddLayer(const Layer& layer) = 0;
	virtual void RemoveLayer(size_t index) = 0;
	virtual size_t GetNumLayers() const = 0;
	virtual void SetNumLayers(size_t num_layers) = 0;
	virtual Layer& GetLayer(size_t index) = 0;

	virtual IGapi* GetGapi() = 0;

	///////////////////////////////////////
	// interact
	virtual void Update(float deltaTime) = 0;
};


} // namespace