#include "Scene.h"


////////////////////////////////////////////////////////////////////////////////
// ctor, dtor, release

Scene::Scene() {
	return;
}

Scene::~Scene() {
	for (auto v : entities) {
		delete v;
	}
	for (auto v : lights) {
		delete v;
	}
}

void Scene::release() {
	delete this;
}


Entity* Scene::createEntity() {
	return nullptr;
}

Light* Scene::createLight() {
	return nullptr;
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