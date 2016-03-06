#pragma once
#include "WorldComponent.h"


namespace graphics { class IEntity; }

class MeshComponent : public WorldComponent
{
public:
	static const eWorldComponentType TYPE = MESH;

public:
	inline MeshComponent(graphics::IEntity* e);

	inline graphics::IEntity* GetEntity();

protected:
	graphics::IEntity* entity;
};


MeshComponent::MeshComponent(graphics::IEntity* e)
:WorldComponent(TYPE), entity(e)
{

}

graphics::IEntity* MeshComponent::GetEntity()
{
	return entity;
}