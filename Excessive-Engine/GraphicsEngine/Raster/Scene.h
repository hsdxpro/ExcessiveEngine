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
	void Release() override;

	// scene content creation
	graphics::Entity* AddEntity() override;
	Light* AddLight() override;
	void Remove(graphics::IEntity* entity) override;
	void Remove(graphics::ILight* light) override;

	void clear() override;

	void SetCamera(graphics::ICamera* c) override;

	graphics::ICamera* GetCamera() override;

	// access content
	const std::unordered_set<graphics::Entity*>& GetEntities();
	const std::unordered_set<Light*>& GetLights();	
private:
	std::unordered_set<graphics::Entity*> entities;
	std::unordered_set<Light*> lights;

	graphics::ICamera* cam;
};