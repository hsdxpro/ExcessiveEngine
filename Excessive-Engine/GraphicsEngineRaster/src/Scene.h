#pragma once

#include "../GraphicsEngine_Interface/interface/IScene.h"
#include <unordered_set>

#include "Entity.h"
#include "Light.h"
#include "../GraphicsEngine_Interface/interface/ICamera.h"

class Scene : public graphics::IScene
{
public:
	// ctor, dtor, 
	Scene();
	~Scene();

	// scene content creation
	graphics::Entity* createEntity() override;
	Light* createLight() override;
	void erase(graphics::IEntity* entity) override;
	void erase(graphics::ILight* light) override;

	void clear() override;

	void setCam(graphics::ICamera* c) override;

	graphics::ICamera* getCam() override;

	// access content
	const std::unordered_set<graphics::Entity*>& getEntities();
	const std::unordered_set<Light*>& getLights();	
private:
	std::unordered_set<graphics::Entity*> entities;
	std::unordered_set<Light*> lights;

	graphics::ICamera* cam;
};