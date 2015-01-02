#include "SoundEngineSFML.h"

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
EXPORT sound::IEngine* CreateSoundEngine(const rSoundEngine& d) {
	return new SoundEngineSFML(d);
}

SoundEngineSFML::SoundEngineSFML(const rSoundEngine& d) {
}

SoundEngineSFML::~SoundEngineSFML() {
}


void SoundEngineSFML::release() {
	delete this;
}

void SoundEngineSFML::update(float deltaTime) {
	
}