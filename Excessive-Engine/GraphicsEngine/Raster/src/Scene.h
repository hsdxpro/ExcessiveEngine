#pragma once
#include "Entity.h"
#include "Light.h"
#include "../../Interfaces/IScene.h"

#include <unordered_set>


class Scene : public graphics::IScene
{
public:
	// ctor, dtor, 
	Scene();
	~Scene();
	void release() override;

	// scene content creation
	graphics::Entity* addEntity() override;
	Light* addLight() override;
	void remove(graphics::IEntity* entity) override;
	void remove(graphics::ILight* light) override;

	void clear() override;

	void setCamera(graphics::ICamera* c) override;

	graphics::ICamera* getCamera() override;

	// access content
	const std::unordered_set<graphics::Entity*>& getEntities();
	const std::unordered_set<Light*>& getLights();	
private:
	std::unordered_set<graphics::Entity*> entities;
	std::unordered_set<Light*> lights;

	graphics::ICamera* cam;
};