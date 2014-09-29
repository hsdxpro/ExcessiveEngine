#pragma once

// namespace
namespace ge {

class IMesh;
class IMaterial;

////////////////////////////////////////////////////////////////////////////////
///	Entity is a geometrical object of a graphical scene.
/// This is only a base object, and is meant for inheriting only!
////////////////////////////////////////////////////////////////////////////////


class IEntity
{
public:	
	virtual void setPos() = 0;
	virtual void setRot() = 0;
	virtual void setScale() = 0;

	virtual void setMesh(IMesh* mesh) = 0;
	virtual void setMaterial(IMaterial* material) = 0;
	virtual IMesh* getMesh() const = 0;
	virtual IMaterial* getMaterial() const = 0;
};



} // namespace