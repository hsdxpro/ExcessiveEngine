#pragma once
#include "..\ISoundData.h"

#include "SFMLCommonSoundData.h"


class SoundData : public sound::ISoundData {
public:
	SoundData();
	~SoundData();
	
	SoundData(const SoundData&) = delete;
	SoundData& operator=(const SoundData&) = delete;

	void Acquire();
	void Release() override;
	
	bool Load(const wchar_t* file_path, sound::StoreMode mode = sound::StoreMode::AUTOMATIC) override;

	const SFMLCommonSoundData* GetSFMLSoundData() const;
	
private:
	size_t refcount;
	SFMLCommonSoundData* pSFMLSoundData; //TODO probably needs to be renamed, due to being very similar to the typename "SFMLSoundData", but has a different meaning
};
