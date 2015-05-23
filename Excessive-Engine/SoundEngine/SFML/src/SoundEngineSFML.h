#pragma once
#include "../../Interfaces/IEngine.h"

#include "SoundScene.h"
#include "SoundData.h"
#include "Listener.h"

struct rSoundEngine {

};

class SoundEngineSFML : public sound::IEngine
{
public:
	// ctor, dtor, release
	SoundEngineSFML(const rSoundEngine& d);
	~SoundEngineSFML();

	void release() override;
	
	SoundScene* createScene() override;
	
	SoundData* createSoundData() override;
	
	Listener* createListener() override;
	
	void setMasterVolume(float volume) override;
	
	float getMasterVolume() const override;

	// interact
	void update(float deltaTime) override;

private:
	float asd;
};

