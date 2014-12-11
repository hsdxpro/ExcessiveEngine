#pragma once

#include "../GraphicsEngine_Interface/interface/IScene.h"
#include <unordered_set>

#include "Entity.h"
#include "Light.h"


class Scene : public graphics::IScene
{
public:
	// ctor, dtor, 
	Scene();
	~Scene();

	// scene content creation
	Entity* createEntity() override;
	Light* createLight() override;
	void erase(graphics::IEntity* entity) override;
	void erase(graphics::ILight* light) override;

	void clear() override;

	// access content
	const std::unordered_set<Entity*>& getEntities();
	const std::unordered_set<Light*>& getLights();	
private:
	std::unordered_set<Entity*> entities;
	std::unordered_set<Light*> lights;
};