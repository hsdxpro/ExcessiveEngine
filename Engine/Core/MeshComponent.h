#pragma once
#include "WorldComponent.h"


namespace graphics { class IEntity; }

class MeshComponent : public WorldComponent
{
public:
	MeshComponent(graphics::IEntity* e);

	__inline graphics::IEntity* GetEntity() { return entity; }

protected:
	void _InnerReflectPos() override;
	void _InnerReflectRot() override;
	void _InnerReflectSkew() override;

protected:
	graphics::IEntity* entity;
};