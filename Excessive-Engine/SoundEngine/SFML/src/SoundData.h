#pragma once

#include "../../Interfaces/ISoundData.h"

#include "SFMLCommonSoundData.h"

class SoundData : public sound::ISoundData {
public:
	SoundData();
	~SoundData();
	
	SoundData(const SoundData&) = delete;
	SoundData& operator=(const SoundData&) = delete;

	void acquire();
	void release() override;
	
	bool load(const wchar_t* file_path, sound::StoreMode mode = sound::StoreMode::AUTOMATIC) override;

	const SFMLCommonSoundData* getSFMLSoundData() const;
	
private:
	size_t refcount;
	SFMLCommonSoundData* pSFMLSoundData; //TODO probably needs to be renamed, due to being very similar to the typename "SFMLSoundData", but has a different meaning
};
