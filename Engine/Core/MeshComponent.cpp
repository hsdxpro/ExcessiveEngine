#include "MeshComponent.h"
#include "GraphicsEngine\IEntity.h"

MeshComponent::MeshComponent(graphics::IEntity* e) 
:entity(e)
{

}

void MeshComponent::_InnerReflectPos()
{
	entity->SetPos(transform.GetPos());
}

void MeshComponent::_InnerReflectRot()
{
	entity->SetRot(transform.GetRot());
}

void MeshComponent::_InnerReflectSkew()
{
	entity->SetSkew(transform.GetSkew());
}