#include "Entity.h"
#include "Material.h"

using namespace graphics;

////////////////////////////////////////////////////////////////////////////////
// Ctor & Dtor + Misc

Entity::Entity()
{
}

Entity::~Entity() {
	return;
}

void Entity::SetPos(const mm::vec3& v)
{
	transform.SetPos(v);
}

void Entity::SetRot(const mm::quat& q)
{
	transform.SetRot(q);
}

void Entity::SetScale(const mm::vec3& v)
{
	transform.SetScale(v);
}

void Entity::SetSkew(const mm::mat3& m)
{
	transform.SetSkew(m);
}

const mm::vec3& Entity::GetPos()
{
	return transform.GetPos();
}
const mm::quat& Entity::GetRot()
{
	return transform.GetRot();
}

const mm::vec3 Entity::GetScale()
{
	return transform.GetScale();
}

const mm::mat3& Entity::GetSkew()
{
	return transform.GetSkew();
}

////////////////////////////////////////////////////////////////////////////////
// mesh & mtl properties

void Entity::SetMesh(graphics::IMesh* mesh) {
	SetMesh((Mesh*)mesh);
}

void Entity::SetMaterial(graphics::IMaterial* mtl) {
	SetMaterial((Material*)mtl);
}

void Entity::SetMesh(Mesh* mesh) {
	this->mesh.Reset(mesh);
}

void Entity::SetMaterial(Material* material) {
	this->mtl.Reset(material);
}

Mesh* Entity::GetMesh() const {
	return mesh.get();
}

Material* Entity::GetMaterial() {
	return mtl.get();
}