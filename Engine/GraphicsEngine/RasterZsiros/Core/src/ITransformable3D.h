// ITransform3D.h By Zsíroskenyér Team 2013.11.13 9:35
// TMP SOLUTION FOR Differently Implemented Entity transforms
#pragma once

#include "math/vec3.h"
#include "math/quat.h"
#include "math/Matrix44.h"

class ITransformable3D {
public:
	virtual void SetPos(const Vec3& p) = 0;
	virtual void SetPos(float x, float y, float z) = 0;

	virtual void SetRot(const Quat& r) = 0;
	virtual void SetRot(float x, float y, float z, float w) = 0;


	virtual void SetScale(const Vec3& s) = 0;
	virtual void SetScale(float x, float y, float z) = 0;

	virtual void SetWorldTransform(ITransformable3D* t);

	virtual const Vec3& GetPos() = 0;
	virtual const Quat& GetRot() = 0;
	virtual const Vec3& GetScale() = 0;
};
