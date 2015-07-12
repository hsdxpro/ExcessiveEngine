#include "Listener.h"

#include "SFML/Audio/Listener.hpp"

#include <cmath>

Listener::Listener() : dir(0, 0, -1), upwards(0, 1, 0){
}

void Listener::SetHandedness(sound::eHandedness newHandedness)  {
	//TODO move handedness to engine class
	handedness = handedness;
}

void Listener::SetUpwards(const mm::vec3& newUp) {
	upwards = mm::normalize(newUp);
}

void Listener::SetTarget(const mm::vec3& newTargetPos) {
	dir = mm::normalize(newTargetPos - this->pos);
}

void Listener::SetDir(const mm::vec3& newDir) {
	dir = mm::normalize(newDir);
}

void Listener::SetPos(const mm::vec3& newPos) {
	pos = newPos;
}

void Listener::SetVel(const mm::vec3& newVel) {
	vel = newVel;
}

sound::eHandedness Listener::GetHandedness() const {
	return handedness;
}

mm::vec3 Listener::GetUpwards() const {
	return upwards;
}

mm::vec3 Listener::GetDir() const {
	return dir;
}

mm::vec3 Listener::GetPos() const {
	return pos;
}

mm::vec3 Listener::GetVel() const {
	return vel;
}

/*
mm::mat4 Listener::GetSFMLViewTransform() const {

	//SFML has a right handed coordinate system where +Y is always up

	//lets say that listener is always at origo
	mm::mat4 result = mm::create_translation(-pos);

	mm::vec3 left = mm::cross(upwards, dir);
	mm::vec3 actualUpwards = mm::cross(dir, left);

	float angleFromUpToY = std::acos(mm::dot(actualUpwards, mm::vec3(0, 1, 0)));

	//rotate everything so that Y will be UP
	//TODO check if Create rotation really takes angle in radians
	result *= mm::create_rotation(angleFromUpToY, mm::cross(actualUpwards, mm::vec3(0, 1, 0)));

	//at this point "listener direction" is on the x z pane, because the actual up direction was used for rotation.
	//now rotate everything to make -Z forward (OpenGL style and i dont care)

	mm::vec3 currentDir = mm::normalize(mm::vec3(dir.x, 0, dir.z));

	float angleFromDirToNegZ = std::acos(mm::dot(currentDir, mm::vec3(0, 0, -1)));

	result *= mm::create_rotation(angleFromDirToNegZ, mm::cross(currentDir, mm::vec3(0, 0, -1)));

	//TODO handle handedness (what a good pun :D)

	return result;
}
*/
