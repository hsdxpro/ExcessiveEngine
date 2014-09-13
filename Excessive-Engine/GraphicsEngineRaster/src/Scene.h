#pragma once

#include "../GraphicsEngine_Interface/interface/IScene.h"


class Scene : ge::IScene
{
public:
	void release() override;

	void add(ge::IEntity* entity) override;
	void add(ge::ILight* light) override;
	void erase(ge::IEntity* entity) override;
	void erase(ge::ILight* light) override;
};