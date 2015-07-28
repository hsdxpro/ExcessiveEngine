#pragma once
#include "mymath/mymath.h"
#include "SupportLibrary/BasicTypes.h"

namespace physics {

class IRigidBodyEntity
{
public:
	virtual void AddForce(const mm::vec3& force, const mm::vec3& relPos = {0,0,0}) = 0;

	virtual void SetGravityScale(float s) = 0;
	virtual void SetTrigger(bool bTrigger) = 0;
	virtual void SetCollisionGroup(i64 ID) = 0;

	virtual void SetAngularFactor(float factor) = 0;
	virtual void SetKinematic(bool bKinematic) = 0;

	virtual void SetPos(const mm::vec3& v) = 0;
	virtual void SetRot(const mm::quat& q) = 0;
	virtual void SetScaleLocal(const mm::vec3& v) = 0;

	virtual const mm::vec3 GetPos() const = 0;
	virtual const mm::quat GetRot() const = 0;
	virtual const mm::vec3 GetScaleLocal() const = 0;

	virtual i64 GetCollisionGroup() const = 0;
	virtual bool GetIsTrigger() const = 0;
};
}