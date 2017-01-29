#pragma once

#include "SFMLCommonSoundEmitter.h"

#include "SFML/Audio.hpp"
#include "mymath/mymath.h"

class SFMLMusicWrapper : public SFMLCommonSoundEmitter
{
public:
	SFMLMusicWrapper();
	bool LoadFromFile(const std::string& filename);

	void SetPitch(float pitch) override;
	void SetVolume(float volume) override;
	void SetPosition(const mm::vec3& newPos) override;
	void SetLooped(bool looped) override;
	
	void Start() override;
	void Pause() override;
	void Stop() override;

	float GetPitch() const override;
	float GetVolume() const override;
	mm::vec3 GetPosition() const override;
	bool GetLooped() const override;

private:
	sf::Music music;
};
