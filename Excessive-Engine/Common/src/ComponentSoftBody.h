#pragma once
#include "WorldComponent.h"

namespace physics { class IEntitySoft; }

class ComponentSoftBody : public WorldComponent
{
public:
	ComponentSoftBody(physics::IEntitySoft* e) :entitySoft(e){}

protected:
	void _InnerUpdatePos() override;
	void _InnerUpdateRot() override;
	void _InnerUpdateScale() override;
	void _InnerUpdateSkew() override;

protected:
	physics::IEntitySoft* entitySoft;
};