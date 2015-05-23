#pragma once

#include "SoundData.h"
#include "../../Interfaces/IEmitter.h"

#include <mymath/mymath.h>

class Scene;

class Emitter : public sound::IEmitter {
public:
	Emitter();
	~Emitter();

	void setPos(const mm::vec3&) override;
	void setVel(const mm::vec3&) override;
	
	mm::vec3 getPos() const override;
	mm::vec3 getVel() const override;
	
	void setSoundData(sound::ISoundData* data) override;
	
	SoundData* getSoundData() const override;
	
protected:
	mm::vec3 pos;
	mm::vec3 vel;
	
// TODO: SoundData reference. (ref_ptr from graphics engine is needed)
};

