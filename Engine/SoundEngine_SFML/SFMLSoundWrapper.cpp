#include "SFMLSoundWrapper.h"

SFMLSoundWrapper::SFMLSoundWrapper(const sf::SoundBuffer& buffer) : sound(buffer){
}

void SFMLSoundWrapper::SetPitch(float pitch) {
	sound.setPitch(pitch);
}

void SFMLSoundWrapper::SetVolume(float volume) {
	sound.setVolume(volume * 100);
}

void SFMLSoundWrapper::SetPosition(const mm::vec3& newPos) {
	sound.setPosition(newPos.x, newPos.y, newPos.z);
}

void SFMLSoundWrapper::SetLooped(bool looped) {
	sound.setLoop(looped);
}

void SFMLSoundWrapper::Start() {
	sound.play();
}

void SFMLSoundWrapper::Pause() {
	sound.pause();
}

void SFMLSoundWrapper::Stop() {
	sound.stop();
}

float SFMLSoundWrapper::GetPitch() const {
	return sound.getPitch();
}

float SFMLSoundWrapper::GetVolume() const {
	return sound.getVolume() / 100.f;
}

mm::vec3 SFMLSoundWrapper::GetPosition() const {
	sf::Vector3f pos = sound.getPosition();
	return mm::vec3(pos.x, pos.y, pos.z);
}

bool SFMLSoundWrapper::GetLooped() const {
	return sound.getLoop();
}

