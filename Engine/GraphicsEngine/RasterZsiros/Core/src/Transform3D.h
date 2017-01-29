// Transform3D.h By Zsíroskenyér Team 2013.11.12 19:59
// Yap now we need one common thing between 3D things :D
#pragma once

#include "math/vec3.h"
#include "math/quat.h"
#include "math/Matrix44.h"

class cTransform3D {
public:
	cTransform3D();
	
	Matrix44 GetWorldMatrix() const;
	
	Vec3 pos;
	Quat rot;
	Vec3 scale;
};