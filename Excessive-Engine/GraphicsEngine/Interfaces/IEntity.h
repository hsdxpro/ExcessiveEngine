#pragma once

#include <mymath/mymath.h>
#include "../Common/src/WorldComponent.h"

// namespace
namespace graphics {

class IMesh;
class IMaterial;

////////////////////////////////////////////////////////////////////////////////
///	Entity is a geometrical object of a graphical scene.
/// This is only a base object, and is meant for inheriting only!
////////////////////////////////////////////////////////////////////////////////


class IEntity
{
public:
	virtual void SetPos(const mm::vec3& v) = 0;
	virtual void SetRot(const mm::quat& q) = 0;
	virtual void SetScale(const mm::vec3& v) = 0;
	virtual void SetSkew(const mm::mat3& m) = 0;

	virtual const mm::vec3& GetPos() = 0;
	virtual const mm::quat& GetRot() = 0;
	virtual const mm::vec3& GetScale() = 0;
	virtual const mm::mat3& GetSkew() = 0;

	virtual void SetMesh(IMesh* mesh) = 0;
	virtual void SetMaterial(IMaterial* material) = 0;
	virtual IMesh* GetMesh() const = 0;
	virtual IMaterial* GetMaterial() const = 0;
};



} // namespace