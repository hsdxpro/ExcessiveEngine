#include "Light.h"

Light::Light()
:color(1,1,1), pos(0,0,0), shadowMap(0),direction(0, 1, 0)
{
	shadowMap = new cShadowMap();
	shadowMap->Init(gGapi, 4096, eFormat::R24_UNORM_X8_TYPELESS, eFormat::D24_UNORM_S8_UINT, 4);
}

Light::~Light()
{
	shadowMap->Clear();
	SAFE_DELETE( shadowMap );
}

void Light::SetColor(const mm::vec3& color)
{
	this->color = color;
}

void Light::SetPos(const mm::vec3& pos)
{
	this->pos = pos;
}

void Light::SetDirection(const mm::vec3& dir)
{
	this->direction = dir;
}

mm::vec3 Light::GetColor() const
{
	return color;
}

mm::vec3 Light::GetPos() const
{
	return pos;
}

mm::vec3 Light::GetDirection() const
{
	return direction;
}