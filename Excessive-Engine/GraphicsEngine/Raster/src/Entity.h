#pragma once

#include "Mesh.h"
#include "Material.h"
#include "../../Interfaces/IEntity.h"

#include "utility/ref_ptr.h"
#include <mymath/mymath.h>


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

	mm::vec3 getPos() override;
	mm::quat getRot() override;
	mm::vec3 getScale() override;

	virtual void setMesh(graphics::IMesh* mesh) override;
	virtual void setMaterial(graphics::IMaterial* mtl) override;
	void setMesh(Mesh* mesh);
	void setMaterial(Material* material);
	Mesh* getMesh() const override;
	Material* getMaterial() const override;

protected:
	// pos-rot-scale
	mm::vec3 pos;
	mm::quat rot;
	mm::vec3 scale;

private:
	// mtl
	ref_ptr<Mesh> mesh;
	ref_ptr<Material> mtl;
};

} // graphics namespace end