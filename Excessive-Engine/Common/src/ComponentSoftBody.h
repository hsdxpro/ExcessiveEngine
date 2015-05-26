#pragma once
#include "WorldComponent.h"

namespace physics { class IEntitySoft; }

class ComponentSoftBody : public WorldComponent
{
public:
	static u8 componentTypeID;
	ComponentSoftBody(physics::IEntitySoft* e) :entitySoft(e){}

protected:
	physics::IEntitySoft* entitySoft;
};