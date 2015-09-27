#include "Scene.h"

using namespace graphics;

////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

Scene::Scene() 
:cam(nullptr) {
	return;
}

Scene::~Scene() {
	clear();
}

void Scene::Release() {
	delete this;
}

////////////////////////////////////////////////////////////////////////////////
// scene content Add/erase

Entity* Scene::AddEntity() {
	Entity* e = new Entity;
	entities.insert(e);
	return e;
}

Light* Scene::AddLight() {
	Light* l = new Light;
	lights.insert(l);
	return l;
}


void Scene::Remove(graphics::IEntity* entity) {
	auto it = entities.find((Entity*)entity);
	if (it != entities.end()) {
		delete *it;
		entities.erase(it);
	}
}

void Scene::Remove(graphics::ILight* light) {
	auto it = lights.find((Light*)light);
	if (it != lights.end()) {
		delete *it;
		lights.erase(it);
	}
}


void Scene::clear()
{
	for (auto v : entities) {
		delete v;
	}
	for (auto v : lights) {
		delete v;
	}
	entities.clear();
	lights.clear();
}

void Scene::SetCamera(graphics::ICamera* c) {
	cam = c;
}

graphics::ICamera* Scene::GetCamera() {
	return cam;
}

const std::unordered_set<Entity*>& Scene::GetEntities() {
	return entities;
}
const std::unordered_set<Light*>& Scene::GetLights() {
	return lights;
}