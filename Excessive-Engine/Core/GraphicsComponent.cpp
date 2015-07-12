#include "GraphicsComponent.h"
#include "GraphicsEngine\IEntity.h"

GraphicsComponent::GraphicsComponent(graphics::IEntity* e) 
:entityGraphics(e)
{

}

void GraphicsComponent::_InnerReflectPos()
{
	entityGraphics->SetPos(transform.GetPos());
}

void GraphicsComponent::_InnerReflectRot()
{
	entityGraphics->SetRot(transform.GetRot());
}

void GraphicsComponent::_InnerReflectSkew()
{
	entityGraphics->SetSkew(transform.GetSkew());
}