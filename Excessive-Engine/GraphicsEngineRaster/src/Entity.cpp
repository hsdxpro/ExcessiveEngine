#include "Entity.h"
#include "Material.h"


////////////////////////////////////////////////////////////////////////////////
// Ctor & Dtor + Misc

Entity::Entity() : pos(0,0,0), rot(0, mm::vec3(1,0,0)), scale(1,1,1)
{

}

Entity::~Entity() {
	return;
}


////////////////////////////////////////////////////////////////////////////////
// transformation data
void Entity::setPos(const mm::vec3& p) {
	pos = p;
}

void Entity::setRot(const mm::quat& r) {
	rot = r;
}

void Entity::setScale(const mm::vec3& s) {
	scale = s;
}


////////////////////////////////////////////////////////////////////////////////
// mesh & mtl properties

void Entity::setMesh(ge::IMesh* mesh) {
	setMesh((Mesh*)mesh);
}

void Entity::setMaterial(ge::IMaterial* mtl) {
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