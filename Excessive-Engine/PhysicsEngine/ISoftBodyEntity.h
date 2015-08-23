#pragma once
#include "mymath/mymath.h"
#include "SupportLibrary/BasicTypes.h"

namespace physics {

class ISoftBodyEntity
{
public:
	virtual void* GetUserPointer() = 0;
	virtual i64 GetCollisionGroup() const = 0;
};

} // namspace physics