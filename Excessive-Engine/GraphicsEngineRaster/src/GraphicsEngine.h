#include "../GraphicsEngine_Interface/interface/IGraphicsEngine.h"

#include <unordered_set>

// must be included because of covariant return type override
#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Camera.h"


////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine implementation.
///
////////////////////////////////////////////////////////////////////////////////

class GraphicsEngineRaster : public graphics::IGraphicsEngine
{
public:
	// ctor, dtor, release
	GraphicsEngineRaster(const graphics::rGraphicsEngine& d);
	~GraphicsEngineRaster();
	bool isConstructionSucceeded() const { return isValid; }

	void release() override;

	// create stuff
	Scene* createScene() override;
	void deleteScene(graphics::IScene* scene) override;
	Mesh* createMesh() override;
	Material* createMaterial() override;
	Texture* createTexture() override;

	Camera* createCam() override;
	// states
	// empty


	// interact
	void update() override;

	IGapi* getGapi();

private:
	std::unordered_set<Scene*> scenes;
	/*
	I don't think these are gonna be necessary
	std::unordered_set<Mesh*> meshes;
	std::unordered_set<Material*> materials;
	std::unordered_set<Texture*> textures;
	*/

	IGapi* gapi;

	// WARNING: temporary code
	IShaderProgram* shader;

	// WARNING: pretty crappy solution
	bool isValid;
};

