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
	virtual void Release() = 0;

	virtual IEntity* AddEntity() = 0;
	virtual ILight* AddLight() = 0;
	virtual void Remove(IEntity* entity) = 0;
	virtual void Remove(ILight* light) = 0;

	virtual void clear() = 0;	

	virtual void SetCamera(ICamera* c) = 0;

	virtual ICamera* GetCamera() = 0;
};


} // namespace