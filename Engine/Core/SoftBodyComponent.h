#pragma once
#include "WorldComponent.h"

namespace physics { class ISoftBodyEntity; }

class SoftBodyComponent : public WorldComponent
{
public:
	static const eWorldComponentType TYPE = SOFT_BODY;

public:
	inline SoftBodyComponent(physics::ISoftBodyEntity* e);

	inline physics::ISoftBodyEntity* GetEntity();

protected:
	physics::ISoftBodyEntity* entity;
};

SoftBodyComponent::SoftBodyComponent(physics::ISoftBodyEntity* e)
:WorldComponent(TYPE), entity(e)
{
}

physics::ISoftBodyEntity* SoftBodyComponent::GetEntity()
{ 
	return entity; 
}