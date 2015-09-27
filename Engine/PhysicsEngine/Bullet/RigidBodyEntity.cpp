#include "RigidBodyEntity.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "SupportLibrary/VisualCpuProfiler.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"

using namespace physics::bullet;

RigidBodyEntity::RigidBodyEntity(btRigidBody* body) 
:body(body), collisionGroupID(-1), userPointer(0) // -1 default means can collide with everything
{
	body->setUserPointer(this);

	//btConvexHullShape asd;
	//asd.getVertex
	//body->getCollisionShape()
	//btBvhTriangleMeshShape asd;
	//asd.getMeshInterface()->getLockedVertexIndexBase
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

void RigidBodyEntity::SetSkew(const mm::mat3& skew)
{
	btCollisionShape* baseShape = body->getCollisionShape();
	int shapeType = baseShape->getShapeType();

	switch (shapeType)
	{
	case CONVEX_HULL_SHAPE_PROXYTYPE:
	{
		btConvexHullShape* shape = (btConvexHullShape*)body->getCollisionShape();

		btVector3* vertices = shape->getUnscaledPoints();
		for (size_t i = 0; i < shape->getNumPoints(); i++)
		{
			mm::vec3 transformedVertex = skew * mm::vec3(vertices[i].x(), vertices[i].y(), vertices[i].z());
			vertices[i] = btVector3(transformedVertex.x, transformedVertex.y, transformedVertex.z);

			shape->recalcLocalAabb();
		}
	}break;
	case CAPSULE_SHAPE_PROXYTYPE:
	{
		btCapsuleShape* shape = (btCapsuleShape*)body->getCollisionShape();

		mm::vec3 localScale = skew * mm::vec3(1, 1, 1);

		shape->setLocalScaling({ localScale.x, localScale.y, localScale.z });
	}break;

	case BOX_SHAPE_PROXYTYPE:
	case TRIANGLE_SHAPE_PROXYTYPE:
	case TETRAHEDRAL_SHAPE_PROXYTYPE:
	case CONVEX_TRIANGLEMESH_SHAPE_PROXYTYPE:
	case CONVEX_POINT_CLOUD_SHAPE_PROXYTYPE:
	case CUSTOM_POLYHEDRAL_SHAPE_TYPE:
	case IMPLICIT_CONVEX_SHAPES_START_HERE:
	case SPHERE_SHAPE_PROXYTYPE:
	case MULTI_SPHERE_SHAPE_PROXYTYPE:
	case CONE_SHAPE_PROXYTYPE:
	case CONVEX_SHAPE_PROXYTYPE:
	case CYLINDER_SHAPE_PROXYTYPE:
	case UNIFORM_SCALING_SHAPE_PROXYTYPE:
	case MINKOWSKI_SUM_SHAPE_PROXYTYPE:
	case MINKOWSKI_DIFFERENCE_SHAPE_PROXYTYPE:
	case BOX_2D_SHAPE_PROXYTYPE:
	case CONVEX_2D_SHAPE_PROXYTYPE:
	case CUSTOM_CONVEX_SHAPE_TYPE:
	case CONCAVE_SHAPES_START_HERE:
	case TRIANGLE_MESH_SHAPE_PROXYTYPE:
	case SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE:
	case FAST_CONCAVE_MESH_PROXYTYPE:
	case TERRAIN_SHAPE_PROXYTYPE:
	case GIMPACT_SHAPE_PROXYTYPE:
	case MULTIMATERIAL_TRIANGLE_MESH_PROXYTYPE:
	case EMPTY_SHAPE_PROXYTYPE:
	case STATIC_PLANE_PROXYTYPE:
	case CUSTOM_CONCAVE_SHAPE_TYPE:
	case CONCAVE_SHAPES_END_HERE:
	case COMPOUND_SHAPE_PROXYTYPE:
	case SOFTBODY_SHAPE_PROXYTYPE:
	case HFFLUID_SHAPE_PROXYTYPE:
	case HFFLUID_BUOYANT_CONVEX_SHAPE_PROXYTYPE:
	case INVALID_SHAPE_PROXYTYPE:
	case MAX_BROADPHASE_COLLISION_TYPES:
		break;
	}
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