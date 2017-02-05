#pragma once
#include "mymath/mymath.h"
#include "BaseLibrary/Types.h"

namespace physics {

class ISoftBodyEntity
{
public:
	virtual void* GetUserPointer() = 0;
	virtual u64 GetCollisionGroup() const = 0;
};

} // namspace physics