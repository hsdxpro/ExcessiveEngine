#include "SoundEngineSFML.h"
#include <iostream>

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
	return nullptr;// new SoundEngineSFML(d);
}

SoundEngineSFML::SoundEngineSFML(const rSoundEngine& d) {
}

SoundEngineSFML::~SoundEngineSFML() {
}


void SoundEngineSFML::release() {
	delete this;
}

SoundScene* SoundEngineSFML::createScene(){
	return nullptr;
}

SoundData* SoundEngineSFML::createSoundData() {
	return nullptr;
}

Listener* SoundEngineSFML::createListener() {
	return nullptr;
}

void SoundEngineSFML::setMasterVolume(float volume) {
}

float SoundEngineSFML::getMasterVolume() const {
	//TODO
	return 0;
}


void SoundEngineSFML::update(float deltaTime) {
	//TODO commit, push audio engine!!
}
