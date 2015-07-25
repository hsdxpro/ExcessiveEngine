#include "GraphicsComponent.h"
#include "GraphicsEngine\IEntity.h"

GraphicsComponent::GraphicsComponent(graphics::IEntity* e) 
:entity(e)
{

}

void GraphicsComponent::_InnerReflectPos()
{
	entity->SetPos(transform.GetPos());
}

void GraphicsComponent::_InnerReflectRot()
{
	entity->SetRot(transform.GetRot());
}

void GraphicsComponent::_InnerReflectSkew()
{
	entity->SetSkew(transform.GetSkew());
}