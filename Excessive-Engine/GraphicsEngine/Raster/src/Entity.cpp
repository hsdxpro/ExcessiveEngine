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