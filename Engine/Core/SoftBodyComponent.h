#pragma once
#include "WorldComponent.h"

namespace physics { class ISoftBodyEntity; }

class SoftBodyComponent : public WorldComponent
{
public:
	SoftBodyComponent(physics::ISoftBodyEntity* e) :entity(e){}

	_inline physics::ISoftBodyEntity* GetEntity() { return entity; }

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	physics::ISoftBodyEntity* entity;
};