#include "SFMLMusicWrapper.h"

#include "SFML/Audio.hpp"

SFMLMusicWrapper::SFMLMusicWrapper() {
	music.setVolume(100);
	music.setAttenuation(0.5);
}

bool SFMLMusicWrapper::loadFromFile(const std::string& filename){
	return music.openFromFile(filename);
}

void SFMLMusicWrapper::setPitch(float pitch) {
	music.setPitch(pitch);
}

void SFMLMusicWrapper::setVolume(float volume) {
	music.setVolume(volume * 100);
}

void SFMLMusicWrapper::setPosition(const mm::vec3& newPos) {
	music.setPosition(newPos.x, newPos.y, newPos.z);
}

void SFMLMusicWrapper::setLooped(bool looped) {
	music.setLoop(looped);
}

void SFMLMusicWrapper::start() {
	music.play();
}

void SFMLMusicWrapper::pause() {
	music.pause();
}

void SFMLMusicWrapper::stop() {
	music.stop();
}

float SFMLMusicWrapper::getPitch() const {
	return music.getPitch();
}

float SFMLMusicWrapper::getVolume() const {
	return music.getVolume() / 100.f;
}

mm::vec3 SFMLMusicWrapper::getPosition() const {
	sf::Vector3f pos = music.getPosition();
	return mm::vec3(pos.x, pos.y, pos.z);
}

bool SFMLMusicWrapper::getLooped() const {
	return music.getLoop();
}
