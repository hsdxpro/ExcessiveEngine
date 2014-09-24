#pragma once

#include <mymath/mymath.h>

#include "../GraphicsEngine_Interface/interface/IEntity.h"

#include "utility/ref_ptr.h"
#include "Mesh.h"
#include "Material.h"

class Scene;


class Entity : public ge::IEntity
{
public:
	Entity();
	~Entity();
	
	void setPosition() override;
	void setRotation() override;
	void setScale() override;

	virtual void setMesh(ge::IMesh* mesh) override;
	virtual void setMaterial(ge::IMaterial* mtl) override;
	void setMesh(Mesh* mesh);
	void setMaterial(Material* material);
	Mesh* getMesh() const override;
	Material* getMaterial() const override;

private:
	ref_ptr<Mesh> mesh;
	ref_ptr<Material> mtl;
};