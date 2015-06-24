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

	void SetPos(const mm::vec3& v);
	void SetRot(const mm::quat& q);
	void SetScaleLocal(const mm::vec3& v);
	void SetSkew(const mm::mat3& m);
	
	const mm::vec3& GetPos();
	const mm::quat& GetRot();
	const mm::vec3& GetScaleLocal();
	const mm::mat3& GetSkew();

	virtual void SetMesh(graphics::IMesh* mesh) override;
	virtual void SetMaterial(graphics::IMaterial* mtl) override;
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	Mesh* GetMesh() const override;
	Material* GetMaterial() const override;

private:
	// mtl
	ref_ptr<Mesh> mesh;
	ref_ptr<Material> mtl;

	Transform3D transform;
};

} // graphics namespace end