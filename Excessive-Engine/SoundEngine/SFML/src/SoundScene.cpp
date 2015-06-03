#include "SoundScene.h"

const mm::vec3 SoundScene::SFMLforwards = mm::vec3(0, 0, -1);
const mm::vec3 SoundScene::SFMLupwards = mm::vec3(0, 1, 0);

SoundScene::SoundScene() :activeListener(nullptr) {
}

SoundScene::~SoundScene() {
	for (auto current : emitters) {
		delete current;
	}
}

void SoundScene::release() {
	delete this;
}

Emitter* SoundScene::addEmitter() {
	Emitter* newEmitter = new Emitter;
	emitters.insert(newEmitter);
	return newEmitter;
}

void SoundScene::remove(sound::IEmitter* emitter) {
	auto it = emitters.find(static_cast<Emitter*>(emitter));
	if (it != emitters.end()){
		delete *it;
		emitters.erase(it);
	}
}

void SoundScene::clear() {
	for (auto current : emitters) {
		delete current;
	}
	emitters.clear();
}

void SoundScene::setListener(sound::IListener* newListener) {
	activeListener = static_cast<Listener*>(newListener);
}

sound::IListener* SoundScene::getListener() const {
	return activeListener;
}

void SoundScene::update(float deltaTime) {

	if (activeListener != nullptr){
		//TODO iterate through all the emitters and set their position according to the current listener setup
		//(note: this is necesarry because SFML alway interprets +y as up, so it is not compatible with all coordinate system out of the box (not even with right handed wich is what sfml uses))
		mm::mat4 listenerTransform = getSFMLViewTransform();
		for (auto current : emitters) {
			mm::vec3 transformedPos = (listenerTransform * mm::vec4(current->getPos(), 1)).xyz;
			current->getSFMLSoundSource()->setPosition(transformedPos);
		}

		sf::Listener::setPosition(0, 0, 0);
		mm::vec3 forwards = mm::transpose(mm::inverse(listenerTransform)) * activeListener->getDir();
		sf::Listener::setDirection(forwards.x, forwards.y, forwards.z);
		//sf::Listener::setDirection(SFMLforwards.x, SFMLforwards.y, SFMLforwards.z);
	}
}

mm::mat4 SoundScene::getSFMLViewTransform() const {
	mm::mat4 result;

	if (activeListener != nullptr){
		//SFML has a right handed coordinate system where +Y is always up

		mm::mat4 upwardsRotation;

		mm::vec3 left = mm::normalize(mm::cross(activeListener->getUpwards(), activeListener->getDir()));
		mm::vec3 actualUpwards = mm::cross(activeListener->getDir(), left);

		float angleFromUpToSFMLUp = std::acos(mm::dot(actualUpwards, SFMLupwards));

		//rotate everything so that SFMLupwards will be UP
		if (mm::impl::is_eq(angleFromUpToSFMLUp, mm::pi) == true) {
			upwardsRotation = mm::create_rotation(angleFromUpToSFMLUp, SFMLforwards);
		}
		else {
			mm::vec3 upwardsRotationAxis = mm::cross(actualUpwards, SFMLupwards);
			if (mm::impl::is_eq(mm::length(upwardsRotationAxis), 0.f) == false) {
				upwardsRotation = mm::create_rotation(angleFromUpToSFMLUp, mm::normalize(upwardsRotationAxis));
			}
		}

		/*
		mm::mat4 directionRotation;
		//at this point "listener direction" is on the x z pane, because the actual up direction was used for rotation.
		//now rotate everything to make -Z forward (OpenGL style and i dont care)

		//NOTE dont need the inverse transposed of the matrix, cause it does not contain scaling,
		mm::vec3 directionWithSFMLUp = upwardsRotation * activeListener->getDir();

		float angleFromDirToSFMLForward = std::acos(mm::dot(directionWithSFMLUp, SFMLforwards));

		//FIXME pi is defined incorrectly
		if (mm::impl::is_eq(angleFromDirToSFMLForward, mm::pi) == true) {
			directionRotation = mm::create_rotation(angleFromDirToSFMLForward, SFMLupwards);
		}
		else {
			mm::vec3 directionRotationAxis = mm::cross(directionWithSFMLUp, SFMLforwards);
			if (mm::impl::is_eq(mm::length(directionRotationAxis), 0.f) == false) {
				directionRotation = mm::create_rotation(angleFromDirToSFMLForward, mm::normalize(directionRotationAxis));
			}
		}
		*/

		//lets say that listener is always at origo
		//result = directionRotation * upwardsRotation * mm::create_translation(activeListener->getPos()*-1);
		result = upwardsRotation * mm::create_translation(activeListener->getPos()*-1);

		//TODO handle handedness (what a good pun :D)
	}

	return result;
}
