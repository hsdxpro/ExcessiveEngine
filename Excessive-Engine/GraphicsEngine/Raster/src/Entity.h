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

	void setPos(const mm::vec3& v);
	void setRot(const mm::quat& q);
	void setScale(const mm::vec3& v);

	const mm::vec3& getPos();
	const mm::quat& getRot();
	const mm::vec3& getScale();

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

	// World Transform
	mm::vec3 pos;
	mm::vec3 scale;
	mm::quat rot;
};

} // graphics namespace end