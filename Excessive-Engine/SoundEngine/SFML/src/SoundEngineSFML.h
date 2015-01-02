#pragma once
#include "../../Interfaces/IEngine.h"

struct rSoundEngine {

};

class SoundEngineSFML : public sound::IEngine
{
public:
	// ctor, dtor, release
	SoundEngineSFML(const rSoundEngine& d);
	~SoundEngineSFML();

	void release() override;

	// interact
	void update(float deltaTime) override;

private:
};

