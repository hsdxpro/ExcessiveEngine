#pragma once
#include "WorldComponent.h"

namespace physics { class ISoftEntity; }

class SoftBodyComponent : public WorldComponent
{
public:
	SoftBodyComponent(physics::ISoftEntity* e) :SoftEntity(e){}

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	physics::ISoftEntity* SoftEntity;
};