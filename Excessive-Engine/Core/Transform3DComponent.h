#pragma once
#include "WorldComponent.h"

class Transform3DComponent : public WorldComponent
{
	void _InnerReflectPos() {};
	void _InnerReflectRot() {};
	void _InnerReflectSkew() {};
};