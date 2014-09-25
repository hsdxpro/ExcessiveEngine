#include "Entity.h"
#include "Material.h"


////////////////////////////////////////////////////////////////////////////////
// Ctor & Dtor + Misc

Entity::Entity() {

}

Entity::~Entity() {

}


////////////////////////////////////////////////////////////////////////////////
// transformation data
void Entity::setPosition() {
	return;
}

void Entity::setRotation() {
	return;
}

void Entity::setScale() {
	return;
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