#pragma once

#include "../GraphicsEngine_Interface/interface/IMeshEntity.h"
#include "Entity.h"


class MeshEntity : public ge::IMeshEntity, public Entity
{
public:
	void setMesh(ge::IMesh* mesh) override;
};