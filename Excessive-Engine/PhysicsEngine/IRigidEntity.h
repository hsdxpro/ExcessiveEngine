#pragma once
#include "mymath/mymath.h"

namespace physics {

class IRigidEntity
{
public:
	virtual void AddForce(const mm::vec3& force, const mm::vec3& relPos = {0,0,0}) = 0;

	virtual void SetAngularFactor(float factor) = 0;
	virtual void SetKinematic() = 0;

	virtual void SetPos(const mm::vec3& v) = 0;
	virtual void SetRot(const mm::quat& q) = 0;
	virtual void SetScaleLocal(const mm::vec3& v) = 0;

	virtual const mm::vec3 GetPos() const = 0;
	virtual const mm::quat GetRot() const = 0;
	virtual const mm::vec3 GetScaleLocal() const = 0;
};
}