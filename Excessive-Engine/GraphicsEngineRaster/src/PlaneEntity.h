#pragma once

#include "../GraphicsEngine_Interface/interface/IPlaneEntity.h"
#include "Entity.h"


class PlaneEntity : public ge::IPlaneEntity, public Entity
{
public:
	void setHeightMap(ge::ITexture* texture) override;
};