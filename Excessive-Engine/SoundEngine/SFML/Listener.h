#pragma once
#include "..\IListener.h"

class Listener : public sound::IListener {
public:
	Listener();

	void SetHandedness(sound::eHandedness handedness) override;
	void SetUpwards(const mm::vec3&) override;
	void SetTarget(const mm::vec3&) override;
	void SetDir(const mm::vec3&) override;
	void SetPos(const mm::vec3&) override;
	void SetVel(const mm::vec3&) override;

	sound::eHandedness GetHandedness() const override;
	mm::vec3 GetUpwards() const override;
	mm::vec3 GetDir() const override;
	mm::vec3 GetPos() const override;
	mm::vec3 GetVel() const override;

	//Get the transformation that moves an Emitter from the current listener coordinate system to the corresponding SFML coordinate system
	//mm::mat4 GetSFMLViewTransform() const;

private:
	sound::eHandedness handedness;

	mm::vec3 upwards;
	mm::vec3 dir;
	mm::vec3 pos;
	mm::vec3 vel;
};
