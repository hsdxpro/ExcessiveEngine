#pragma once

#include "KeyframeAnimation.h"
#include "../GraphicsEngine/Interfaces/IEntity.h"



// reference coordinate system is:
// y bone forward
// z bone up
// x bone right

class Animator {
	using Keyframe = KeyframeAnimation::Keyframe;
	using Bone = KeyframeAnimation::Bone;
public:
	// Set global parameters
	void setEntity(graphics::IEntity* entity);
	graphics::IEntity* getEntity() const;

	void setAnimation(KeyframeAnimation* animation);
	KeyframeAnimation* getAnimation() const;

	/// Returns weather the animation and the entity are compatible
	bool isCompatible() const;

	// Set animation state
	void setPlayTime(float time);
	float getPlayTime() const { return playTime; }
	void update(float elapsed);
	void setPlaySpeed(float speed);
	float getPlaySpeed() const { return playSpeed; }

protected:
	// Helper methods
	Keyframe lerp(const Keyframe& kf1, const Keyframe& kf2, float t) const;
	Keyframe cubic(size_t index1, size_t index2, size_t index3, size_t index4, float t);
	std::vector<mm::mat4> absolute(const Keyframe& keyframe);
	static inline mm::mat4 matrixify(mm::vec3 pos, mm::quat rot, mm::vec3 scale) {
		mm::mat4 pM = mm::create_translation(pos);
		mm::mat4 rM = rot;
		mm::mat4 sM = mm::create_scale(scale);
		return pM*rM*sM;
	}
private:
	graphics::IEntity* entity;
	KeyframeAnimation* animation;
	float playTime;
	float playSpeed;
};