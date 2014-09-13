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
	virtual void release() = 0;
	
	virtual void setPosition() = 0;
	virtual void setRotation() = 0;
	virtual void setScale() = 0;

	virtual void setMaterial(IMaterial* material) = 0;
};



} // namespace