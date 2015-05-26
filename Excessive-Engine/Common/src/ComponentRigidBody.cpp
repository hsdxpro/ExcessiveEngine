#include "ComponentRigidBody.h"
#include "..\PhysicsEngine\Interfaces\IEntityRigid.h"

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