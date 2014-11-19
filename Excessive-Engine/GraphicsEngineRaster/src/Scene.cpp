#include "Scene.h"


////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

Scene::Scene() {
	return;
}

Scene::~Scene() {
	clear();
}

void Scene::release() {
	delete this;
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


void Scene::erase(ge::IEntity* entity) {
	auto it = entities.find((Entity*)entity);
	if (it != entities.end()) {
		delete *it;
		entities.erase(it);
	}
}

void Scene::erase(ge::ILight* light) {
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
}