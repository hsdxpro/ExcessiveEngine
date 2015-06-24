#pragma once

// namespace
namespace graphics {

////////////////////////////////////////////////////////////////////////////////
///	Light is a light in the graphical scene.
///
////////////////////////////////////////////////////////////////////////////////


class ILight 
{
public:
	virtual void SetColor() = 0;
	virtual void SetPosition() = 0;
	virtual void SetDirection() = 0;
};






} // namespace