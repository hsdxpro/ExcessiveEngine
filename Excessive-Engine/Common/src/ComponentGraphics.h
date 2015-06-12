#pragma once
#include "WorldComponent.h"

namespace graphics { class IEntity; }

class ComponentGraphics : public WorldComponent
{
public:
	ComponentGraphics(graphics::IEntity* e) :entityGraphics(e){}

	void SetPos(const mm::vec3& v);
	void SetRot(const mm::quat& q);
	void SetScale(const mm::vec3& v);

	const mm::vec3& GetPos();
	const mm::quat& GetRot();
	const mm::vec3& GetScale();

protected:
	void _InnerUpdatePos() override;
	void _InnerUpdateRot() override;
	void _InnerUpdateScale() override;
	void _InnerUpdateSkew() override;

protected:
	graphics::IEntity* entityGraphics;
};