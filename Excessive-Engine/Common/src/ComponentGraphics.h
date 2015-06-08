#pragma once
#include "WorldComponent.h"

namespace graphics { class IEntity; }

class ComponentGraphics : public WorldComponent
{
public:
	ComponentGraphics(graphics::IEntity* e) :entityGraphics(e){}

	void setPos(const mm::vec3& v);
	void setRot(const mm::quat& q);
	void setScale(const mm::vec3& v);

	const mm::vec3& getPos();
	const mm::quat& getRot();
	const mm::vec3& getScale();

protected:
	void _innerUpdatePos() override;
	void _innerUpdateRot() override;
	void _innerUpdateScale() override;

protected:
	graphics::IEntity* entityGraphics;
};