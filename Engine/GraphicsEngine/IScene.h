#pragma once
#include "GraphicsEngine_Raster\Entity.h"
#include "SupportLibrary\ICamera.h"

#include <unordered_set>
#include "ILight.h"

// namespace
namespace graphics {

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

	virtual const std::unordered_set<graphics::IEntity*>& GetEntities() const = 0;
	virtual const std::unordered_set<ILight*>& GetLights() const = 0;
};


} // namespace