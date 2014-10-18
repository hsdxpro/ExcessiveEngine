#pragma once

// namespace
namespace ge {

////////////////////////////////////////////////////////////////////////////////
///	Light is a light in the graphical scene.
///
////////////////////////////////////////////////////////////////////////////////


class ILight 
{
public:
	virtual void setColor() = 0;
	virtual void setPosition() = 0;
	virtual void setDirection() = 0;
};






} // namespace