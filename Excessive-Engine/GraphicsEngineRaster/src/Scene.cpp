#include "Scene.h"


////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

Scene::Scene() {
	return;
}

Scene::~Scene() {
	clear();
}


////////////////////////////////////////////////////////////////////////////////
// scene content add/erase

Entity* Scene::createEntity() {
	Entity* e = new Entity;
	entities.insert(e);
	return e;
}

Light* Scene::createLight() {
	Light* l = new Light;
	lights.insert(l);
	return l;
}


void Scene::erase(graphics::IEntity* entity) {
	auto it = entities.find((Entity*)entity);
	if (it != entities.end()) {
		delete *it;
		entities.erase(it);
	}
}

void Scene::erase(graphics::ILight* light) {
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


const std::unordered_set<Entity*>& Scene::getEntities() {
	return entities;
}
const std::unordered_set<Light*>& Scene::getLights() {
	return lights;
}