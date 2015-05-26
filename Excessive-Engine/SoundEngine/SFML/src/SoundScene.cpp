#include "SoundScene.h"

SoundScene::SoundScene():listener(nullptr) {

}

SoundScene::~SoundScene() {

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
	listener = newListener;
}

sound::IListener* SoundScene::getListener() const {
	return listener;
}
