#include "EntityRigid.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

EntityRigid::EntityRigid(btRigidBody* body) 
:body(body) {
}

void EntityRigid::SetPos(const mm::vec3& v)
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	trans.setOrigin(btVector3( v.x, v.y, v.z ));
	body->setWorldTransform(trans);
	body->getMotionState()->setWorldTransform(trans);
}

void EntityRigid::SetRot(const mm::quat& q)
{
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

void EntityRigid::SetScaleLocal(const mm::vec3& v)
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
}

mm::vec3 EntityRigid::GetPos()
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	return{ trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z() };
}

mm::quat EntityRigid::GetRot()
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

 mm::vec3 EntityRigid::GetScaleLocal()
{
	btCollisionShape* shape = body->getCollisionShape();

	if (shape)
		return{ shape->getLocalScaling().x(), shape->getLocalScaling().y(), shape->getLocalScaling().z() };

	return{ 1.f, 1.f, 1.f };
}

 btRigidBody* EntityRigid::GetBody()
 {
	 return body;
 }