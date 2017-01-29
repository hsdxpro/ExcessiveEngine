#pragma once

#include "Mesh.h"
#include "Material.h"
#include "GraphicsEngine/IEntity.h"
#include "SupportLibrary/Transform3D.h"

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
	void SetScale(const mm::vec3& v);
	void SetSkew(const mm::mat3& m);
	
	const mm::vec3& GetPos();
	const mm::quat& GetRot();
	const mm::vec3 GetScale();
	const mm::mat3& GetSkew();

	virtual void SetMesh(graphics::IMesh* mesh) override;
	virtual void SetMaterial(graphics::IMaterial* mtl) override;
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	Mesh* GetMesh() const override;
	Material* GetMaterial() override;

private:
	// mtl
	ref_ptr<Mesh> mesh;
	ref_ptr<Material> mtl;

	Transform3D transform;
};

} // graphics namespace end