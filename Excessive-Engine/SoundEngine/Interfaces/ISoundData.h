#pragma once
#include "../../Common/src/BasicTypes.h"

// namespace
namespace sound {

enum class StoreMode : u32 {
	STREAMED, /// Do not load sound file into memory, stream it from hard drive instead.
	BUFFERED, /// Load complete file into memory.
	AUTOMATIC /// Decide automatically how to load sound.
};

class ISoundData {
public:
	virtual void release() = 0;
	
	virtual bool load(const wchar_t* file_path, StoreMode mode = StoreMode::AUTOMATIC) = 0;
};

} // namespace
