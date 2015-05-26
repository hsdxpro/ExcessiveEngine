#include "SoundEngineSFML.h"
#include <iostream>

#include "SFML\Audio.hpp"

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

SoundEngineSFML::SoundEngineSFML(const rSoundEngine& d):masterVolume(1) {
	sf::Listener::setGlobalVolume(masterVolume * 100);
}

SoundEngineSFML::~SoundEngineSFML() {
}


void SoundEngineSFML::release() {
	delete this;
}

SoundScene* SoundEngineSFML::createScene(){
	return new SoundScene;
}

SoundData* SoundEngineSFML::createSoundData() {
	return new SoundData;
}

Listener* SoundEngineSFML::createListener() {
	return new Listener;
}

void SoundEngineSFML::setMasterVolume(float volume) {
	masterVolume = volume;
}

float SoundEngineSFML::getMasterVolume() const {
	return masterVolume;
}

void SoundEngineSFML::update(float deltaTime) {

}
