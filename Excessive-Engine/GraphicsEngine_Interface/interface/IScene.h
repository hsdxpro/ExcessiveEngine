#pragma once

// namespace
namespace graphics {

class IEntity;
class ILight;

////////////////////////////////////////////////////////////////////////////////
/// Represents a single graphical scene.
///
////////////////////////////////////////////////////////////////////////////////


class IScene
{
public:
	virtual IEntity* createEntity() = 0;
	virtual ILight* createLight() = 0;
	virtual void erase(IEntity* entity) = 0;
	virtual void erase(ILight* light) = 0;

	virtual void clear() = 0;	
};


} // namespace