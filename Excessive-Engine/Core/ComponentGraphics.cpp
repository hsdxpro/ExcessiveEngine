#include "ComponentGraphics.h"
#include "GraphicsEngine\IEntity.h"

ComponentGraphics::ComponentGraphics(graphics::IEntity* e) 
:entityGraphics(e)
{

}

void ComponentGraphics::_InnerReflectPos()
{
	entityGraphics->SetPos(transform.GetPos());
}

void ComponentGraphics::_InnerReflectRot()
{
	entityGraphics->SetRot(transform.GetRot());
}

void ComponentGraphics::_InnerReflectSkew()
{
	entityGraphics->SetSkew(transform.GetSkew());
}