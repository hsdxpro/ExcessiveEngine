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

void Scene::release() {
	delete this;
}

////////////////////////////////////////////////////////////////////////////////
// scene content add/erase

Entity* Scene::addEntity() {
	Entity* e = new Entity;
	entities.insert(e);
	return e;
}

Light* Scene::addLight() {
	Light* l = new Light;
	lights.insert(l);
	return l;
}


void Scene::remove(graphics::IEntity* entity) {
	auto it = entities.find((Entity*)entity);
	if (it != entities.end()) {
		delete *it;
		entities.erase(it);
	}
}

void Scene::remove(graphics::ILight* light) {
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

void Scene::setCamera(graphics::ICamera* c) {
	cam = c;
}

graphics::ICamera* Scene::getCamera() {
	return cam;
}

const std::unordered_set<Entity*>& Scene::getEntities() {
	return entities;
}
const std::unordered_set<Light*>& Scene::getLights() {
	return lights;
}