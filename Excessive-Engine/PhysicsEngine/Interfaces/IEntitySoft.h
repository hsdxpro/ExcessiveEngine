#pragma once
#include "mymath/mymath.h"
#include "../Common/src/WorldComponent.h"

namespace physics {

class IEntitySoft : WorldComponent
{
public:
	virtual void setPos(const mm::vec3& p) = 0;
	virtual void setRot(const mm::quat& r) = 0;
	virtual void setScale(const mm::vec3& s) = 0;

	virtual mm::vec3 getPos() = 0;
	virtual mm::quat getRot() = 0;
	virtual mm::vec3 getScale() = 0;
};
}