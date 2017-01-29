// IPhysicsEntity.h By Zsíroskenyér Team 2013.10.25 23:41
#pragma once

#include "math\vec3.h"
#include "math\quat.h"

class IPhysicsEntity {
public:
	virtual void SetPos(const Vec3& p) = 0;
	virtual void SetPos(float x, float y, float z) = 0;

	virtual void SetRot(const Quat& r) = 0;
	virtual void SetRot(float x, float y, float z, float w) = 0;


	virtual void SetScale(const Vec3& s) = 0;
	virtual void SetScale(float x, float y, float z) = 0;

	virtual const Vec3& GetPos() const = 0;
	virtual const Quat& GetRot() const = 0;
	virtual const Vec3& GetScale() const = 0;

	virtual void Release() = 0;
};