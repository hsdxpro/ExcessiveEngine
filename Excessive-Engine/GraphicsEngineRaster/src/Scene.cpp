#include "Scene.h"


void Scene::release() {
	delete this;
}

void Scene::add(ge::IEntity* entity) {
}
void Scene::add(ge::ILight* light) {
}
void Scene::erase(ge::IEntity* entity) {
}
void Scene::erase(ge::ILight* light) {
}