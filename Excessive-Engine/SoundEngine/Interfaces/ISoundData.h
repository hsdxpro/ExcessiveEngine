#pragma once
#include "../../Common/src/BasicTypes.h"

// namespace
namespace sound {

class ISoundData {
public:
	virtual void release() = 0;

	enum StoreMode : u32 {
		STREAMED, /// Do not load sound file into memory, stream it from hard drive instead.
		BUFFERED, /// Load complete file into memory.
		AUTOMATIC, /// Decide automatically how to load sound.
	};
	
	virtual bool load(const wchar_t* file_path, StoreMode mode = AUTOMATIC) = 0; 
};

} // namespace
