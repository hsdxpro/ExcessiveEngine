#include "../GraphicsEngine_Interface/interface/IGraphicsEngine.h"


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

};