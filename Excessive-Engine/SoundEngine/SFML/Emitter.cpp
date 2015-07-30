#include "Emitter.h"

#include "SoundData.h"
#include "SFMLMusicWrapper.h"

#include <mymath/mymath.h>

Emitter::Emitter() {
	//TODO initialize sound source to a valid, but meaningless instance, so that it doesnt have to check for null pointer every time
	pSFMLSoundSource = nullptr;
	bEmitting = false;
}

Emitter::~Emitter() {
	delete pSFMLSoundSource;
}

void Emitter::SetPos(const mm::vec3& newPos) {
	pos = newPos;
}

void Emitter::SetVel(const mm::vec3& newVel) {
	vel = newVel;
}

void Emitter::SetLooped(bool looped) {
	pSFMLSoundSource->SetLooped(looped);
}

void Emitter::SetVolume(float volume) {
	pSFMLSoundSource->SetVolume(volume);
}

void Emitter::Start() {
	pSFMLSoundSource->Start();
	bEmitting = true;
}

void Emitter::Pause() {
	pSFMLSoundSource->Pause();
	bEmitting = false;
}

void Emitter::Stop() {
	pSFMLSoundSource->Stop();
	bEmitting = false;
}

mm::vec3 Emitter::GetPos() const {
	return pos;
}

mm::vec3 Emitter::GetVel() const {
	return vel;
}

float Emitter::GetVolume() const {
	return pSFMLSoundSource->GetVolume() / 100.f;
}

bool Emitter::GetLooped() const {
	return pSFMLSoundSource->GetLooped();
}

bool Emitter::IsEmitting() const {
	return bEmitting;
}

void Emitter::SetSoundData(sound::ISoundData* data)  {
	//FIXME error occurs in this function!
	SoundData* SFMLData = static_cast<SoundData*>(data);
	assert(pSFMLSoundSource == 0);
	delete pSFMLSoundSource; //FIXME error occurs at this deletion (virtual destructor used)
	pSFMLSoundSource = SFMLData->GetSFMLSoundData()->CreateSFMLSoundSource();

	soundData.Reset(SFMLData);
}

SoundData* Emitter::GetSoundData() {
	return soundData.get();
}

SFMLCommonSoundEmitter* Emitter::GetSFMLSoundSource() {
	return pSFMLSoundSource;
}
