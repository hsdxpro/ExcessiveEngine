#include "KeyframeAnimation.h"
#include <algorithm>


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Keyframe

void KeyframeAnimation::Keyframe::setNumBones(size_t n) {
	bones.resize(n);
}
size_t KeyframeAnimation::Keyframe::getNumBones() const {
	return bones.size();
}

void KeyframeAnimation::Keyframe::setDuration(float duration) {
	this->duration = max(0.0f, duration);
}
float KeyframeAnimation::Keyframe::getDuration() const {
	return duration;
}

auto KeyframeAnimation::Keyframe::operator[](size_t i) -> Bone& {
	return bones[i];
}
auto KeyframeAnimation::Keyframe::operator[](size_t i) const -> const Bone& {
	return bones[i];
}

KeyframeAnimation::Bone* KeyframeAnimation::Keyframe::data() {
	return bones.data();
}
const KeyframeAnimation::Bone* KeyframeAnimation::Keyframe::data() const {
	return bones.data();
}


////////////////////////////////////////////////////////////////////////////////
// KeyframeAnimation

KeyframeAnimation::KeyframeAnimation() {

}

// Modify KeyframeAnimation
bool KeyframeAnimation::setLayout(size_t* childCounts, size_t numBones) {
	// check weather structure is correct
	bool isCorrect = TraverseLinearTree(childCounts, numBones, [](size_t, size_t) {});

	// set internal stuff
	if (isCorrect) {
		layout.assign(childCounts, childCounts + numBones);
		initBoneCount();
	}

	return isCorrect;
}

const size_t* KeyframeAnimation::getChildCounts() const {
	return layout.data();
}

size_t KeyframeAnimation::getNumBones() const {
	return layout.size();
}


void KeyframeAnimation::setNumKeyframes(size_t n) {
	keyframes.resize(n);
	initBoneCount();
}

size_t KeyframeAnimation::getNumKeyframes() const {
	return keyframes.size();
}

const KeyframeAnimation::Keyframe& KeyframeAnimation::operator[](size_t i) const {
	return keyframes[i];
}



const KeyframeAnimation::Keyframe& KeyframeAnimation::getDefaultState() const {
	return defaultBones;
}

KeyframeAnimation::Bone* KeyframeAnimation::getDefaultBones() {
	return defaultBones.data();
}


KeyframeAnimation::Bone* KeyframeAnimation::getBones(size_t keyframe) {
	return keyframes[keyframe].data();
}
void KeyframeAnimation::setKeyframeDuration(size_t keyframe, float duration) {
	keyframeTimesDirty = true;
	keyframes[keyframe].setDuration(duration);
}
float KeyframeAnimation::getKeyframeDuration(size_t keyframe) {
	return keyframes[keyframe].getDuration();
}


void KeyframeAnimation::initBoneCount() {
	for (auto& v : keyframes) {
		v.setNumBones(getNumBones());
	}
	defaultBones.setNumBones(getNumBones());
	keyframeTimes.resize(getNumBones());
}


void KeyframeAnimation::recalcKeyframeTimes() {
	if (keyframeTimes.size() > 0) {
		keyframeTimes[0] = 0.0f;
	}
	for (ptrdiff_t i = 0; i < (ptrdiff_t)keyframes.size() - 1; ++i) {
		keyframeTimes[i + 1] = keyframeTimes[i] + keyframes[i].getDuration();
	}
}


size_t KeyframeAnimation::getKeyframeAt(float timePoint) {
	if (keyframeTimesDirty) {
		recalcKeyframeTimes();
	}

	// TODO: implement binary search!!
	ptrdiff_t i;
	for (i = 0; i < (ptrdiff_t)keyframeTimes.size() - 1; i++) {
		if (keyframeTimes[i] <= timePoint && keyframeTimes[i + 1] > timePoint) {
			break;
		}
	}
	return i;
}
