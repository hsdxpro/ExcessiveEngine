#pragma once
#include "SoundEngine/ISoundEngine.h"

#include "SoundScene.h"
#include "SoundData.h"
#include "Listener.h"


struct rSoundEngine {

};

class SoundEngineSFML : public ISoundEngine
{
public:
	// ctor, dtor, release
	SoundEngineSFML(const rSoundEngine& d);
	~SoundEngineSFML();

	void Release() override;
	
	SoundScene* CreateScene() override;
	
	SoundData* CreateSoundData() override;
	
	Listener* CreateListener() override;

	void SetScene(sound::IScene* scene) override;
	
	void SetMasterVolume(float volume) override;
	
	float GetMasterVolume() const override;

	// interact
	void Update(float deltaTime) override;

private:
	std::unordered_set<SoundScene*> scenes;
	std::unordered_set<SoundData*> soundDataObjects;
	std::unordered_set<Listener*> listeners;

	SoundScene* activeScene;
};

