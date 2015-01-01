#pragma once
#include "../Module_Interfaces/SoundEngine/ISoundEngine.h"

struct rSoundEngine {

};

class SoundEngine : public sound::ISoundEngine
{
public:
	// ctor, dtor, release
	SoundEngine(const rSoundEngine& d);
	~SoundEngine();

	void release() override;

	// interact
	void update() override;

private:
};

