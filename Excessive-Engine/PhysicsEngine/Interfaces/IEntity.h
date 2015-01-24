#pragma once
#include "mymath/mymath.h"

namespace physics {

class IEntity
{
public:
	virtual void setPos(const mm::vec3& p) = 0;
	virtual void setRot(const mm::quat& r) = 0;

	virtual mm::vec3 getPos() = 0;
	virtual mm::quat getRot() = 0;
};

}