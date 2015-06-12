#include "Entity.h"
#include "Material.h"

using namespace graphics;

////////////////////////////////////////////////////////////////////////////////
// Ctor & Dtor + Misc

Entity::Entity()
:scale(1,1,1)
{
}

Entity::~Entity() {
	return;
}

void Entity::SetPos(const mm::vec3& v)
{
	pos = v;
}

void Entity::SetRot(const mm::quat& q)
{
	rot = q;
}

void Entity::SetScale(const mm::vec3& v)
{
	scale = v;
}

void Entity::SetSkew(const mm::mat3& m)
{
	skew = m;
}

const mm::vec3& Entity::GetPos()
{
	return pos;
}
const mm::quat& Entity::GetRot()
{
	return rot;
}

const mm::vec3& Entity::GetScale()
{
	return scale;
}

const mm::mat3& Entity::GetSkew()
{
	return skew;
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

Material* Entity::GetMaterial() const {
	return mtl.get();
}