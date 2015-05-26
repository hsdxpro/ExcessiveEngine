#include "SFMLSoundWrapper.h"

SFMLSoundWrapper::SFMLSoundWrapper(const sf::SoundBuffer& buffer) : sound(buffer){
}

void SFMLSoundWrapper::setPitch(float pitch) {
	sound.setPitch(pitch);
}

void SFMLSoundWrapper::setVolume(float volume) {
	sound.setVolume(volume * 100);
}

void SFMLSoundWrapper::setPosition(const mm::vec3& newPos) {
	sound.setPosition(newPos.x, newPos.y, newPos.z);
}

void SFMLSoundWrapper::start() {
	sound.play();
}

void SFMLSoundWrapper::pause() {
	sound.pause();
}

void SFMLSoundWrapper::stop() {
	sound.stop();
}

float SFMLSoundWrapper::getPitch() const {
	return sound.getPitch();
}

float SFMLSoundWrapper::getVolume() const {
	return sound.getVolume() / 100.f;
}

mm::vec3 SFMLSoundWrapper::getPosition() const {
	sf::Vector3f pos = sound.getPosition();
	return mm::vec3(pos.x, pos.y, pos.z);
}

