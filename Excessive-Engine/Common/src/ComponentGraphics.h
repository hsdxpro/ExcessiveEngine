#pragma once
#include "WorldComponent.h"

namespace graphics { class IEntity; }

class ComponentGraphics : public WorldComponent
{
public:
	ComponentGraphics(graphics::IEntity* e) :entityGraphics(e){}

	void SetPos(const mm::vec3& v);
	void SetRot(const mm::quat& q);
	void SetScaleLocal(const mm::vec3& v);

	const mm::vec3& GetPos();
	const mm::quat& GetRot();
	const mm::vec3& GetScaleLocal();

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	graphics::IEntity* entityGraphics;
};