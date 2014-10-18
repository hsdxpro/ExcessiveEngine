#include "../GraphicsEngine_Interface/interface/IGraphicsEngine.h"

#include <unordered_set>

// must be included because of covariant return type override
#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"


////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine implementation.
///
////////////////////////////////////////////////////////////////////////////////

class GraphicsEngine : public ge::IGraphicsEngine 
{
public:
	// ctor, dtor, release
	GraphicsEngine();
	~GraphicsEngine();

	void release() override;

	// create stuff
	Scene* createScene() override;
	Mesh* createMesh() override;
	Material* createMaterial() override;
	Texture* createTexture() override;

	// states
		// empty

	// interact
	void update() override;

private:
	std::unordered_set<Scene*> scenes;
	std::unordered_set<Mesh*> meshes;
	std::unordered_set<Material*> materials;
	std::unordered_set<Texture*> textures;
};