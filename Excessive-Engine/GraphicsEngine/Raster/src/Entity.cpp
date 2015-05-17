#include "Entity.h"
#include "Material.h"

using namespace graphics;

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


mm::vec3 Entity::getPos() {
	return pos;
}

mm::quat Entity::getRot() {
	return rot;
}

mm::vec3 Entity::getScale() {
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
	// set bone matrices to correspond to mesh's bone indices
	if (mesh->getHighestBoneIndex() >= 0) {
		boneMatrices.resize(mesh->getHighestBoneIndex() + 1);
		for (auto& v : boneMatrices) {
			v = mm::create_scale({1,1,1}); // identity matrix
		}
	}
	else {
		boneMatrices.clear();
	}
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


////////////////////////////////////////////////////////////////////////////////
// animation shit
mm::mat4* Entity::getBoneMatrices() {
	return boneMatrices.size() > 0 ? boneMatrices.data() : nullptr;
}
const mm::mat4* Entity::getBoneMatrices() const {
	return boneMatrices.size() > 0 ? boneMatrices.data() : nullptr;
}
uint32_t Entity::getNumBones() const {
	return boneMatrices.size();
}