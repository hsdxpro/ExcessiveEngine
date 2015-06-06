#include "Emitter.h"

#include "SoundData.h"
#include "SFMLMusicWrapper.h"

#include <mymath/mymath.h>

Emitter::Emitter() {
	//TODO initialize sound source to a valid, but meaningless instance, so that it doesnt have to check for null pointer every time
	pSFMLSoundSource = nullptr;
}

Emitter::~Emitter() {
	delete pSFMLSoundSource;
}

void Emitter::setPos(const mm::vec3& newPos) {
	pos = newPos;
}

void Emitter::setVel(const mm::vec3& newVel) {
	vel = newVel;
}

void Emitter::setLooped(bool looped) {
	pSFMLSoundSource->setLooped(looped);
}

void Emitter::setVolume(float volume) {
	pSFMLSoundSource->setVolume(volume);
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

float Emitter::getVolume() const {
	return pSFMLSoundSource->getVolume() / 100.f;
}

bool Emitter::getLooped() const {
	return pSFMLSoundSource->getLooped();
}

void Emitter::setSoundData(sound::ISoundData* data)  {
	//FIXME error occurs in this function!
	SoundData* SFMLData = static_cast<SoundData*>(data);
	assert(pSFMLSoundSource == 0);
	delete pSFMLSoundSource; //FIXME error occurs at this deletion (virtual destructor used)
	pSFMLSoundSource = SFMLData->getSFMLSoundData()->createSFMLSoundSource();

	soundData.reset(SFMLData);
}

SoundData* Emitter::getSoundData() {
	return soundData.get();
}

SFMLCommonSoundEmitter* Emitter::getSFMLSoundSource() {
	return pSFMLSoundSource;
}
