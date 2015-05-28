#include "SoundScene.h"

SoundScene::SoundScene() :activeListener(nullptr) {

}

SoundScene::~SoundScene() {
	for (auto current : emitters) {
		delete current;
	}
}

void SoundScene::release() {
	delete this;
}

Emitter* SoundScene::addEmitter() {
	Emitter* newEmitter = new Emitter;
	emitters.insert(newEmitter);
	return newEmitter;
}

void SoundScene::remove(sound::IEmitter* emitter) {
	auto it = emitters.find(static_cast<Emitter*>(emitter));
	if (it != emitters.end()){
		delete *it;
		emitters.erase(it);
	}
}

void SoundScene::clear() {
	for (auto current : emitters) {
		delete current;
	}
	emitters.clear();
}

void SoundScene::setListener(sound::IListener* newListener) {
	activeListener = static_cast<Listener*>(newListener);
}

sound::IListener* SoundScene::getListener() const {
	return activeListener;
}

void SoundScene::update(float deltaTime) {
	if (activeListener != nullptr) {
		//TODO convert position according to current handedness
		mm::vec3 listenerPos = activeListener->getPos();
		sf::Listener::setPosition(listenerPos.x, listenerPos.y, listenerPos.z);
		mm::vec3 listenerDir = activeListener->getDir();
		sf::Listener::setDirection(listenerDir.x, listenerDir.y, listenerDir.z);
	}
}
