#pragma once

#include "../GraphicsEngine_Interface/interface/IScene.h"
#include <unordered_set>

#include "Entity.h"
#include "Light.h"


class Scene : public ge::IScene
{
public:
	// ctor, dtor, 
	Scene();
	~Scene();
	void release() override;

	// scene content creation
	Entity* createEntity() override;
	Light* createLight() override;
	void erase(ge::IEntity* entity) override;
	void erase(ge::ILight* light) override;

	void clear() override;
	
private:
	std::unordered_set<Entity*> entities;
	std::unordered_set<Light*> lights;
};