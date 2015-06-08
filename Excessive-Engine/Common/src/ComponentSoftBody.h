#pragma once
#include "WorldComponent.h"

namespace physics { class IEntitySoft; }

class ComponentSoftBody : public WorldComponent
{
public:
	ComponentSoftBody(physics::IEntitySoft* e) :entitySoft(e){}

protected:
	void _innerUpdatePos() override;
	void _innerUpdateRot() override;
	void _innerUpdateScale() override;

protected:
	physics::IEntitySoft* entitySoft;
};