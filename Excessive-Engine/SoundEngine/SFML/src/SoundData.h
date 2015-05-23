#pragma once

#include "../../Interfaces/ISoundData.h"

class SoundData : public sound::ISoundData {
public:
	SoundData();
	~SoundData();
	
	void aquire();
	void release() override;
	
	bool load(const wchar_t* file_path, sound::ISoundData::StoreMode mode = sound::ISoundData::AUTOMATIC) override;
	
private:
	size_t refcount;
};