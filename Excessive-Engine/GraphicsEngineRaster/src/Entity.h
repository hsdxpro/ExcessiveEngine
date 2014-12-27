#pragma once

#include <mymath/mymath.h>

#include "../GraphicsEngine_Interface/interface/IEntity.h"

#include "utility/ref_ptr.h"
#include "Mesh.h"
#include "Material.h"

class Scene;

namespace graphics
{

class Entity : public graphics::IEntity
{
public:
	Entity();
	~Entity();

	void setPos(const mm::vec3&) override;
	void setRot(const mm::quat&) override;
	void setScale(const mm::vec3&) override;

	virtual void setMesh(graphics::IMesh* mesh) override;
	virtual void setMaterial(graphics::IMaterial* mtl) override;
	void setMesh(Mesh* mesh);
	void setMaterial(Material* material);
	Mesh* getMesh() const override;
	Material* getMaterial() const override;

private:
	// mtl
	ref_ptr<Mesh> mesh;
	ref_ptr<Material> mtl;
	// pos-rot-scale
	mm::vec3 pos;
	mm::quat rot;
	mm::vec3 scale;
};

} // graphics namespace end