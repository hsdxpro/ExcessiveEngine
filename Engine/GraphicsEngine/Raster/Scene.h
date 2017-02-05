#pragma once
#include "Entity.h"
#include "Light.h"
#include "GraphicsEngine/IScene.h"
#include "GraphicsEngine/ILight.h"
#include "BaseLibrary/Camera.h"

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

	void SetCamera(ICamera* c) override;

	ICamera* GetCamera() override;

	// access content
	const std::unordered_set<graphics::IEntity*>& GetEntities() const override;
	const std::unordered_set<graphics::ILight*>& GetLights() const override;

private:
	std::unordered_set<graphics::IEntity*> entities;
	std::unordered_set<graphics::ILight*> lights;

	Camera* cam;
};