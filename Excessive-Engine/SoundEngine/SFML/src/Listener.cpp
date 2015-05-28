#include "Listener.h"

#include "SFML/Audio/Listener.hpp"

void Listener::setHandedness(sound::eHandedness newHandedness)  {
	//TODO move handedness to engine class
	handedness = handedness;
}

void Listener::setUpwards(const mm::vec3& newUp) {
	upwards = newUp;
}

void Listener::setTarget(const mm::vec3& newTargetPos) {
	dir = mm::normalize(newTargetPos - this->pos);
}

void Listener::setDir(const mm::vec3& newDir) {
	dir = mm::normalize(newDir);
}

void Listener::setPos(const mm::vec3& newPos) {
	pos = newPos;
}

void Listener::setVel(const mm::vec3& newVel) {
	vel = newVel;
}

sound::eHandedness Listener::getHandedness() const {
	return handedness;
}

mm::vec3 Listener::getUpwards() const {
	return upwards;
}

mm::vec3 Listener::getDir() const {
	return dir;
}

mm::vec3 Listener::getPos() const {
	return pos;
}

mm::vec3 Listener::getVel() const {
	return vel;
}
