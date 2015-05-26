#include "Emitter.h"

#include "SoundData.h"
#include "SFMLMusicWrapper.h"

#include <mymath/mymath.h>

//initialize sound source to a valid, but meaningless instance, so that it doesnt have to check for null pointer every time
Emitter::Emitter() : pSFMLSoundSource(new SFMLMusicWrapper) {
}

Emitter::~Emitter() {
	delete pSFMLSoundSource;
}

void Emitter::setPos(const mm::vec3& newPos) {
	pos = newPos;
	pSFMLSoundSource->setPosition(pos); //TODO convert to match handedness (not sure if it is needed)
}

void Emitter::setVel(const mm::vec3& newVel) {
	vel = newVel;
}

void Emitter::start() {
	pSFMLSoundSource->start();
}

void Emitter::pause() {
	pSFMLSoundSource->pause();
}

void Emitter::stop() {
	pSFMLSoundSource->stop();
}

mm::vec3 Emitter::getPos() const {
	return pos;
}

mm::vec3 Emitter::getVel() const {
	return vel;
}

void Emitter::setSoundData(sound::ISoundData* data)  {
	SoundData* SFMLData = static_cast<SoundData*>(data);
	delete pSFMLSoundSource;
	pSFMLSoundSource = SFMLData->getSFMLSoundData()->createSFMLSoundSource();

	soundData.reset(SFMLData);
}

SoundData* Emitter::getSoundData() const {
	return soundData.get();
}
