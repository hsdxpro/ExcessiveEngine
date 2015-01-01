#include "SoundEngine.h"

////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C"
EXPORT sound::ISoundEngine* CreateSoundEngine(const rSoundEngine& d) {
	return new SoundEngine(d);
}

SoundEngine::SoundEngine(const rSoundEngine& d) {
}

SoundEngine::~SoundEngine() {
}


void SoundEngine::release() {
	delete this;
}

void SoundEngine::update() {
	
}