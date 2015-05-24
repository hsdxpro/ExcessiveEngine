#pragma once
#include "mymath/mymath.h"
#include "../Common/src/WorldComponent.h"

namespace physics {

class IEntityRigid : public WorldComponent
{
public:
	IEntityRigid():WorldComponent(RIGID){}
};
}