#pragma once
#include "mymath/mymath.h"
#include "SupportLibrary/BasicTypes.h"
#include "PhysicsCommon.h"
#include <vector>

namespace physics {

class IRigidBodyEntity
{
public:
	virtual void AddForce(const mm::vec3& force, const mm::vec3& relPos = {0,0,0}) = 0;

	virtual void SetUserPointer(void* ptr) = 0;
	virtual void SetGravityScale(float s) = 0;
	virtual void SetTrigger(bool bTrigger) = 0;
	virtual void SetCollisionGroup(i64 ID) = 0;

	virtual void SetAngularFactor(float factor) = 0;
	virtual void SetKinematic(bool bKinematic) = 0;
	virtual void SetVelocity(const mm::vec3& v) = 0;

	virtual void SetPos(const mm::vec3& v) = 0;
	virtual void SetRot(const mm::quat& q) = 0;
	virtual void SetScale(const mm::vec3& v) = 0;
	virtual void SetSkew(const mm::mat3& skew) = 0;

	virtual const mm::vec3 GetPos() const = 0;
	virtual const mm::quat GetRot() const = 0;
	virtual const mm::vec3 GetScale() const = 0;

	virtual u64 GetCollisionGroup() const = 0;
	virtual mm::vec3 GetVelocity() const = 0;
	virtual void* GetUserPointer() = 0;

	virtual std::vector<ContactPoint> GetContactPoints() const = 0;

	virtual bool IsTrigger() const = 0;
	virtual bool IsStatic() const = 0;
	virtual bool IsDynamic() const = 0;
	virtual bool IsKinematic() const = 0;
};

} // namespace physics