#pragma once

#include "../../Interfaces/IListener.h"

class Listener : public sound::IListener {
public:
	void setHandedness(sound::eHandedness handedness) override;
	void setUpwards(const mm::vec3&) override;
	void setTarget(const mm::vec3&) override;
	void setDir(const mm::vec3&) override;
	void setPos(const mm::vec3&) override;
	void setVel(const mm::vec3&) override;

	sound::eHandedness getHandedness() const override;
	mm::vec3 getUpwards() const override;
	mm::vec3 getDir() const override;
	mm::vec3 getPos() const override;
	mm::vec3 getVel() const override;

private:
	sound::eHandedness handedness;

	mm::vec3 upwards;
	mm::vec3 dir;
	mm::vec3 pos;
	mm::vec3 vel;
};
