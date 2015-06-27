#include "SoundEngineSFML.h"
#include <iostream>

#include "SFML/Audio.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C"
EXPORT sound::IEngine* CreateSoundEngine(const rSoundEngine& d) {
	return new SoundEngineSFML(d);
}

SoundEngineSFML::SoundEngineSFML(const rSoundEngine& d) : activeScene(nullptr) {
	sf::Listener::setGlobalVolume(100);
}

SoundEngineSFML::~SoundEngineSFML() {
	for (auto current : scenes) {
		delete current;
	}

	for (auto current : soundDataObjects) {
		delete current;
	}

	for (auto current : listeners) {
		delete current;
	}
}


void SoundEngineSFML::Release() {
	delete this;
}

SoundScene* SoundEngineSFML::CreateScene(){
	SoundScene* newScene = new SoundScene;
	scenes.insert(newScene);

	//TODO is it a good idea to have active scene automatically Set?
	if (activeScene == nullptr) {
		activeScene = newScene;
	}
	return newScene;
}

SoundData* SoundEngineSFML::CreateSoundData() {
	SoundData* newData = new SoundData;
	soundDataObjects.insert(newData);
	return newData;
}

Listener* SoundEngineSFML::CreateListener() {
	Listener* newListener = new Listener;
	listeners.insert(newListener);
	return newListener;
}

void SoundEngineSFML::SetScene(sound::IScene* scene) {
	activeScene = static_cast<SoundScene*>(scene);
}

void SoundEngineSFML::SetMasterVolume(float volume) {
	sf::Listener::setGlobalVolume(volume * 100);
}

float SoundEngineSFML::GetMasterVolume() const {
	return sf::Listener::getGlobalVolume() / 100.f;
}

void SoundEngineSFML::Update(float deltaTime) {
	if (activeScene != nullptr) {
		activeScene->Update(deltaTime);
	}
}
