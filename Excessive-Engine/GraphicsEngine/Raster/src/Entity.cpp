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

void Entity::setPos(const mm::vec3& v)
{
	pos = v;
}

void Entity::setRot(const mm::quat& q)
{
	rot = q;
}

void Entity::setScale(const mm::vec3& v)
{
	scale = v;
}

const mm::vec3& Entity::getPos()
{
	return pos;
}
const mm::quat& Entity::getRot()
{
	return rot;
}

const mm::vec3& Entity::getScale()
{
	return scale;
}

////////////////////////////////////////////////////////////////////////////////
// mesh & mtl properties

void Entity::setMesh(graphics::IMesh* mesh) {
	setMesh((Mesh*)mesh);
}

void Entity::setMaterial(graphics::IMaterial* mtl) {
	setMaterial((Material*)mtl);
}

void Entity::setMesh(Mesh* mesh) {
	this->mesh.reset(mesh);
}

void Entity::setMaterial(Material* material) {
	this->mtl.reset(material);
}

Mesh* Entity::getMesh() const {
	return mesh.get();
}

Material* Entity::getMaterial() const {
	return mtl.get();
}