#include "SFMLMusicWrapper.h"

#include "SFML/Audio.hpp"

SFMLMusicWrapper::SFMLMusicWrapper() {
	music.setVolume(100);
	music.setAttenuation(0.1f);
}

bool SFMLMusicWrapper::LoadFromFile(const std::string& filename){
	return music.openFromFile(filename);
}

void SFMLMusicWrapper::SetPitch(float pitch) {
	music.setPitch(pitch);
}

void SFMLMusicWrapper::SetVolume(float volume) {
	music.setVolume(volume * 100);
}

void SFMLMusicWrapper::SetPosition(const mm::vec3& newPos) {
	music.setPosition(newPos.x, newPos.y, newPos.z);
}

void SFMLMusicWrapper::SetLooped(bool looped) {
	music.setLoop(looped);
}

void SFMLMusicWrapper::Start() {
	music.play();
}

void SFMLMusicWrapper::Pause() {
	music.pause();
}

void SFMLMusicWrapper::Stop() {
	music.stop();
}

float SFMLMusicWrapper::GetPitch() const {
	return music.getPitch();
}

float SFMLMusicWrapper::GetVolume() const {
	return music.getVolume() / 100.f;
}

mm::vec3 SFMLMusicWrapper::GetPosition() const {
	sf::Vector3f pos = music.getPosition();
	return mm::vec3(pos.x, pos.y, pos.z);
}

bool SFMLMusicWrapper::GetLooped() const {
	return music.getLoop();
}
