#pragma once
#include "SoundEngine/IEmitter.h"

#include "SoundData.h"
#include "SFMLCommonSoundEmitter.h"

//TODO fix this ugly include
#include <mymath/mymath.h>
#include "GraphicsEngine/Raster/utility/ref_ptr.h"


class Scene;

class Emitter : public sound::IEmitter {
public:
	Emitter();
	~Emitter();

	Emitter(const Emitter& other) = delete;
	Emitter& operator=(const Emitter& other) = delete;

	void SetPos(const mm::vec3& newPos) override;
	void SetVel(const mm::vec3& newVel) override;
	void SetVolume(float volume) override;
	void SetLooped(bool looped) override;

	void Start() override;
	void Pause() override;
	void Stop() override;
	
	mm::vec3 GetPos() const override;
	mm::vec3 GetVel() const override;
	float GetVolume() const override;
	bool GetLooped() const override;
	bool IsEmitting() const override;

	void SetSoundData(sound::ISoundData* data) override;
	
	SoundData* GetSoundData() override;

	SFMLCommonSoundEmitter* GetSFMLSoundSource();
	
protected:
	mm::vec3 pos;
	mm::vec3 vel;
	bool bEmitting;

	ref_ptr<SoundData> soundData;
	SFMLCommonSoundEmitter* pSFMLSoundSource;
};

