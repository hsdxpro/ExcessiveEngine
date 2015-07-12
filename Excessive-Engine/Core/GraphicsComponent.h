#pragma once
#include "WorldComponent.h"


namespace graphics { class IEntity; }

class GraphicsComponent : public WorldComponent
{
public:
	GraphicsComponent(graphics::IEntity* e);

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	graphics::IEntity* entityGraphics;
};