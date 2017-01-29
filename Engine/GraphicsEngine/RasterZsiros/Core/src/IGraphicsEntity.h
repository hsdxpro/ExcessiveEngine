////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Graphics Entity interface
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../../Core/src/math/vec3.h"
#include "../../Core/src/math/quat.h"

class IGraphicsEntity {
public:
	//virtual ~IGraphicsEntity() {}; // should NOT be released or deleted, but RemoveEntitied

	virtual void SetPos(const Vec3& p) = 0;
	virtual void SetPos(float x, float y, float z) = 0;

	virtual void SetRot(const Quat& r) = 0;
	virtual void SetRot(float x, float y, float z, float w) = 0;

	virtual void SetScale(const Vec3& s) = 0;
	virtual void SetScale(float x, float y, float z) = 0;

	virtual const Vec3& GetPos() const = 0;
	virtual const Quat& GetRot() const = 0;
	virtual const Vec3& GetScale() const = 0;
};