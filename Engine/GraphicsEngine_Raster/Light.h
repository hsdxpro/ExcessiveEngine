#pragma once
#include "GraphicsEngine/ILight.h"
#include "GraphicsEngine_RasterZsiros/GraphicsEngineRaster/src/ShadowMap.h"

class Light : public graphics::ILight
{
public:
	Light();
	~Light();

	void SetColor(const mm::vec3& color) override;
	void SetPos(const mm::vec3& pos) override;
	void SetDirection(const mm::vec3& dir) override;

	mm::vec3 GetColor() const override;
	mm::vec3 GetPos() const override;
	mm::vec3 GetDirection() const override;

	cShadowMap* GetShadowMap() const override { return shadowMap; }
protected:
	cShadowMap* shadowMap;

	mm::vec3 direction;
	mm::vec3 color;
	mm::vec3 pos;
};