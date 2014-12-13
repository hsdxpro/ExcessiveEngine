#pragma once

class Camera;

// namespace
namespace graphics {

class IEntity;
class ILight;

////////////////////////////////////////////////////////////////////////////////
/// Represents a si ngle graphical scene.
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

	virtual void setCam(const Camera& c) = 0;

	virtual Camera& getCam() = 0;
};


} // namespace