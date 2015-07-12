#include "RigidEntity.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

RigidEntity::RigidEntity(btRigidBody* body) 
:body(body) 
{
}

void RigidEntity::AddForce(const mm::vec3& force, const mm::vec3& relPos /*= {0,0,0}*/)
{
	body->applyForce({force.x, force.y, force.z}, {relPos.x, relPos.y, relPos.z});
	body->activate();
}

void RigidEntity::SetAngularFactor(float factor)
{
	body->setAngularFactor(factor);
}

void RigidEntity::SetKinematic()
{
	body->setFlags(body->getFlags() | btRigidBody::CF_KINEMATIC_OBJECT);
}

void RigidEntity::SetPos(const mm::vec3& v)
{
	btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		trans.setOrigin(btVector3( v.x, v.y, v.z ));
	body->setWorldTransform(trans);
	body->getMotionState()->setWorldTransform(trans);
	body->activate();
}

void RigidEntity::SetRot(const mm::quat& q)
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	btQuaternion btQuat;
		btQuat.setX(q.value.x);
		btQuat.setY(q.value.y);
		btQuat.setZ(q.value.z);
		btQuat.setW(q.value.w);
	trans.setRotation(btQuat);

	body->setWorldTransform(trans);
	body->getMotionState()->setWorldTransform(trans);
	body->activate();
}

void RigidEntity::SetScaleLocal(const mm::vec3& v)
{
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
	body->activate();
}

const mm::vec3 RigidEntity::GetPos() const
{
	btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
	return mm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
}

const mm::quat RigidEntity::GetRot() const
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);

	mm::quat rot;
		rot.value.x = trans.getRotation().x();
		rot.value.y = trans.getRotation().y();
		rot.value.z = trans.getRotation().z();
		rot.value.w = trans.getRotation().w();
	return rot;
}

 const mm::vec3 RigidEntity::GetScaleLocal() const
{
	btCollisionShape* shape = body->getCollisionShape();

	if (shape)
		return mm::vec3(shape->getLocalScaling().x(), shape->getLocalScaling().y(), shape->getLocalScaling().z());

	return mm::vec3(1,1,1);
}

 btRigidBody* RigidEntity::GetBody()
 {
	 return body;
 }