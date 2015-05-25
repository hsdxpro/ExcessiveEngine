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
	virtual void setPos(const mm::vec3& v) = 0;
	virtual void setRot(const mm::quat& q) = 0;
	virtual void setScale(const mm::vec3& v) = 0;

	virtual const mm::vec3& getPos() = 0;
	virtual const mm::quat& getRot() = 0;
	virtual const mm::vec3& getScale() = 0;

	virtual void setMesh(IMesh* mesh) = 0;
	virtual void setMaterial(IMaterial* material) = 0;
	virtual IMesh* getMesh() const = 0;
	virtual IMaterial* getMaterial() const = 0;
};



} // namespace