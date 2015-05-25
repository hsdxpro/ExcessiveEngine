#pragma once
#include "mymath/mymath.h"
#include "../Common/src/WorldComponent.h"

namespace physics {

class IEntityRigid
{
public:
	virtual void setPos(const mm::vec3& v) = 0;
	virtual void setRot(const mm::quat& q) = 0;
	virtual void setScale(const mm::vec3& v) = 0;

	virtual mm::vec3 getPos() = 0;
	virtual mm::quat getRot() = 0;
	virtual mm::vec3 getScale() = 0;
};
}