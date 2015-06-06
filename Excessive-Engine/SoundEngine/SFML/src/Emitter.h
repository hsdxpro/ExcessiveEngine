#pragma once

#include "../../Interfaces/IEmitter.h"

#include "SoundData.h"
#include "SFMLCommonSoundEmitter.h"

//TODO fix this ugly include
#include "../../../GraphicsEngine/Raster/src/utility/ref_ptr.h"
#include <mymath/mymath.h>

class Scene;

class Emitter : public sound::IEmitter {
public:
	Emitter();
	~Emitter();

	Emitter(const Emitter& other) = delete;
	Emitter& operator=(const Emitter& other) = delete;

	void setPos(const mm::vec3& newPos) override;
	void setVel(const mm::vec3& newVel) override;
	void setVolume(float volume) override;
	void setLooped(bool looped) override;

	void start() override;
	void pause() override;
	void stop() override;
	
	mm::vec3 getPos() const override;
	mm::vec3 getVel() const override;
	float getVolume() const override;
	bool getLooped() const override;

	void setSoundData(sound::ISoundData* data) override;
	
	SoundData* getSoundData() override;

	SFMLCommonSoundEmitter* getSFMLSoundSource();
	
protected:
	mm::vec3 pos;
	mm::vec3 vel;

	ref_ptr<SoundData> soundData;
	SFMLCommonSoundEmitter* pSFMLSoundSource;
};

