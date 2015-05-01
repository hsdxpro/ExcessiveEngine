#pragma once

// namespace
namespace graphics {

class IEntity;
class ILight;
class ICamera;
////////////////////////////////////////////////////////////////////////////////
/// Represents a single graphical scene.
///
////////////////////////////////////////////////////////////////////////////////

class IScene
{
public:
	virtual void release() = 0;

	virtual IEntity* addEntity() = 0;
	virtual ILight* addLight() = 0;
	virtual void remove(IEntity* entity) = 0;
	virtual void remove(ILight* light) = 0;

	virtual void clear() = 0;	

	virtual void setCamera(ICamera* c) = 0;

	virtual ICamera* getCamera() = 0;
};


} // namespace