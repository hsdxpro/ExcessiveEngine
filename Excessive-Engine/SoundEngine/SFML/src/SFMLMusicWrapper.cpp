#include "SFMLMusicWrapper.h"

#include "SFML\Audio.hpp"

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

