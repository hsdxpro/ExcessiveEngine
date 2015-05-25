#include "WorldComponent.h"
#include <assert.h>
#include "Sys.h"
#include "..\GraphicsEngine\Interfaces\IEntity.h"
#include "..\PhysicsEngine\Interfaces\IEntityRigid.h"

// Update this when u add new component type !
enum eTypeID
{
	_0,
	_1,
	_2,

	COUNT,
};

// Update this when u add new component type !
u8 ComponentGraphics::componentTypeID = eTypeID::_0;
u8 ComponentRigidBody::componentTypeID = eTypeID::_1;
u8 ComponentSoftBody::componentTypeID = eTypeID::_2;


Transform::Transform()
	:scale(1, 1, 1) {
}

void Transform::setPos(const mm::vec3& v)
{
	pos = v;
}

void Transform::setScale(const mm::vec3& v)
{
	scale = v;
}

void Transform::setRot(const mm::quat& q)
{
	rot = q;
}

const mm::vec3& Transform::getPos()
{
	return pos;
}

const mm::quat& Transform::getRot()
{
	return rot;
}

const mm::vec3& Transform::getScale()
{
	return scale;
}









WorldComponent::WorldComponent()
{
	componentTypeContainer.resize(eTypeID::COUNT);
}

void WorldComponent::updateAfterPhysicsSimulate()
{

}

// World component graphics
void WorldComponent::setPos(const mm::vec3& v)
{
	pos = v;

	for (auto& compTypes : componentTypeContainer)
		for (auto& comp : compTypes)
			comp->setPos(v);
}

void WorldComponent::setRot(const mm::quat& q)
{
	rot = q;

	for (auto& compTypes : componentTypeContainer)
		for (auto& comp : compTypes)
			comp->setRot(q);
}

void WorldComponent::setScale(const mm::vec3& v)
{
	scale = v;

	for (auto& compTypes : componentTypeContainer)
		for (auto& comp : compTypes)
			comp->setScale(v);
}

const mm::vec3& WorldComponent::getPos()
{
	return pos;
}

const mm::quat& WorldComponent::getRot()
{
	return rot;
}

const mm::vec3& WorldComponent::getScale()
{
	return scale;
}

// Component graphics
void ComponentGraphics::setPos(const mm::vec3& v)
{
	entityGraphics->setPos(v);
	WorldComponent::setPos(v);
}

void ComponentGraphics::setRot(const mm::quat& q)
{
	entityGraphics->setRot(q);
	WorldComponent::setRot(q);
}
void ComponentGraphics::setScale(const mm::vec3& v)
{
	entityGraphics->setScale(v);
	WorldComponent::setScale(v);
}

const mm::vec3& ComponentGraphics::getPos()
{
	return entityGraphics->getPos();
}

const mm::quat& ComponentGraphics::getRot()
{
	return entityGraphics->getRot();
}

const mm::vec3& ComponentGraphics::getScale()
{
	return entityGraphics->getScale();
}



// Component rigidBody
void ComponentRigidBody::updateAfterPhysicsSimulate()
{
	WorldComponent::setPos(entityRigid->getPos());
	WorldComponent::setRot(entityRigid->getRot());
	WorldComponent::setScale(entityRigid->getScale());
}

void ComponentRigidBody::setPos(const mm::vec3& v)
{
	entityRigid->setPos(v);
	WorldComponent::setPos(v);
}

void ComponentRigidBody::setRot(const mm::quat& q)
{
	entityRigid->setRot(q);
	WorldComponent::setRot(q);
}

void ComponentRigidBody::setScale(const mm::vec3& v)
{
	entityRigid->setScale(v);
	WorldComponent::setScale(v);
}

const mm::vec3& ComponentRigidBody::getPos()
{
	return entityRigid->getPos();
}

const mm::quat& ComponentRigidBody::getRot()
{
	return entityRigid->getRot();
}

const mm::vec3& ComponentRigidBody::getScale()
{
	return entityRigid->getScale();
}