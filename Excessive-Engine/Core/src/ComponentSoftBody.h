#pragma once
#include "WorldComponent.h"

namespace physics { class IEntitySoft; }

class ComponentSoftBody : public WorldComponent
{
public:
	ComponentSoftBody(physics::IEntitySoft* e) :entitySoft(e){}

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	physics::IEntitySoft* entitySoft;
};