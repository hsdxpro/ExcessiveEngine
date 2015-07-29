#include "RigidBodyEntity.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

using namespace physics::bullet;

RigidBodyEntity::RigidBodyEntity(btRigidBody* body) 
:body(body), collisionGroupID(-1) // -1 default means can collide with everything
{
	body->setUserPointer(this);
}

void RigidBodyEntity::AddForce(const mm::vec3& force, const mm::vec3& relPos /*= {0,0,0}*/)
{
	body->applyForce({force.x, force.y, force.z}, {relPos.x, relPos.y, relPos.z});
	body->activate();
}

void RigidBodyEntity::SetGravityScale(float s)
{
	body->setGravity(body->getGravity() * s);
}

void RigidBodyEntity::SetTrigger(bool bTrigger)
{
	if (bTrigger)
		body->setCollisionFlags( body->getCollisionFlags() | btRigidBody::CF_NO_CONTACT_RESPONSE);
	else
		body->setCollisionFlags(body->getCollisionFlags() & ~btRigidBody::CF_NO_CONTACT_RESPONSE);
}

void RigidBodyEntity::SetCollisionGroup(i64 ID)
{
	collisionGroupID = ID;
}

void RigidBodyEntity::SetAngularFactor(float f)
{
	body->setAngularFactor(f);
}

void RigidBodyEntity::SetKinematic(bool b)
{
	if (b)
		body->setFlags(body->getFlags() | btRigidBody::CF_KINEMATIC_OBJECT);
	else
		body->setFlags(body->getFlags() & ~btRigidBody::CF_KINEMATIC_OBJECT);
}

void RigidBodyEntity::SetVelocity(const mm::vec3& v)
{
	body->setLinearVelocity({ v.x, v.y, v.z });
}

void RigidBodyEntity::SetPos(const mm::vec3& v)
{
	btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		trans.setOrigin(btVector3( v.x, v.y, v.z ));
	body->setWorldTransform(trans);
	body->getMotionState()->setWorldTransform(trans);
	body->activate();
}

void RigidBodyEntity::SetRot(const mm::quat& q)
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

void RigidBodyEntity::SetScaleLocal(const mm::vec3& v)
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

const mm::vec3 RigidBodyEntity::GetPos() const
{
	btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
	return mm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
}

const mm::quat RigidBodyEntity::GetRot() const
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

 const mm::vec3 RigidBodyEntity::GetScaleLocal() const
{
	btCollisionShape* shape = body->getCollisionShape();

	if (shape)
		return mm::vec3(shape->getLocalScaling().x(), shape->getLocalScaling().y(), shape->getLocalScaling().z());

	return mm::vec3(1,1,1);
}

 btRigidBody* RigidBodyEntity::GetBody()
 {
	 return body;
 }