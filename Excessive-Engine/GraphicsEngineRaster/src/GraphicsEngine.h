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

namespace ge {

class GraphicsEngineRaster : public ge::IGraphicsEngine
{
public:
	// ctor, dtor, release
	GraphicsEngineRaster(const rGraphicsEngine& d);
	~GraphicsEngineRaster();

	//void init(const IGraphicsEngine::rDesc& d);
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

	IGapi* getGapi();

private:
	std::unordered_set<Scene*> scenes;
	std::unordered_set<Mesh*
	> meshes;
	std::unordered_set<Material*> materials;
	std::unordered_set<Texture*> textures;

	IGapi* gapi;
};

} // namespace ge