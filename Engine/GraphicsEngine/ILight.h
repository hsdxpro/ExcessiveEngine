#pragma once
#include "GraphicsEngine/RasterZsiros\GraphicsEngineRaster\src\ShadowMap.h"

// namespace
namespace graphics {

////////////////////////////////////////////////////////////////////////////////
///	Light is a light in the graphical scene.
///
////////////////////////////////////////////////////////////////////////////////

class ILight 
{
public:
	virtual void SetColor(const mm::vec3& color) = 0;
	virtual void SetPos(const mm::vec3& pos) = 0;
	virtual void SetDirection(const mm::vec3& dir) = 0;

	virtual mm::vec3 GetColor() const = 0;
	virtual mm::vec3 GetPos() const = 0;
	virtual mm::vec3 GetDirection() const = 0;

	virtual cShadowMap* GetShadowMap() const = 0;
};






} // namespace