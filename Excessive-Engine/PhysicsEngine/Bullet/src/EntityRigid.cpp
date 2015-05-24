#include "EntityRigid.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

EntityRigid::EntityRigid(btRigidBody* body) 
:body(body) {
}

void EntityRigid::updateAfterSimulate()
{
	// Okay so after physics simulation done we need the simulation of WorldComponent transform, with calling setPos, setRot
	WorldComponent::setPos(getPos());
	WorldComponent::setRot(getRot());	
}

void EntityRigid::setPos(const mm::vec3& v) 
{
	WorldComponent::setPos(v);

	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	trans.setOrigin(btVector3( v.x, v.y, v.z ));

	body->setWorldTransform(trans);
	body->getMotionState()->setWorldTransform(trans);
}

void EntityRigid::setRot(const mm::quat& q)
{
	WorldComponent::setRot(q);

	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	btQuaternion btQuat;
		btQuat.setX(q.vector().x);
		btQuat.setY(q.vector().y);
		btQuat.setZ(q.vector().z);
		btQuat.setW(q.scalar());
	trans.setRotation(btQuat);

	body->setWorldTransform(trans);
	body->getMotionState()->setWorldTransform(trans);
}

void EntityRigid::setScale(const mm::vec3& v)
{
	WorldComponent::setScale(v);

	btCollisionShape* colShape = body->getCollisionShape();

	if (colShape)
	{
		colShape->setLocalScaling(btVector3(v.x, v.y, v.z));
		
		// I think it's needed
		btVector3 localInertia(0, 0, 0);
		float invMass = body->getInvMass();
		if (invMass != 0)
		{
			float mass = 1.f / invMass;
			colShape->calculateLocalInertia(mass, localInertia);
			body->setMassProps(mass, localInertia);
		}
	}
}

const mm::vec3& EntityRigid::getPos() 
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);

	pos.x = trans.getOrigin().x();
	pos.y = trans.getOrigin().y();
	pos.z = trans.getOrigin().z();

	return pos;
}

const mm::quat& EntityRigid::getRot()
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);

	rot.value.x = trans.getRotation().x();
	rot.value.y = trans.getRotation().y();
	rot.value.z = trans.getRotation().z();
	rot.value.w = trans.getRotation().w();

	return rot;
}

const mm::vec3& EntityRigid::getScale()
{
	btCollisionShape* shape = body->getCollisionShape();

	if (shape)
	{
		scale.x = shape->getLocalScaling().x();
		scale.y = shape->getLocalScaling().y();
		scale.z = shape->getLocalScaling().z();
		return scale;
	}

	return WorldComponent::getScale();
}