#pragma once

#include "Emitter.h"
#include "Listener.h"
#include "../../Interfaces/IScene.h"

#include <unordered_set>

class SoundScene : public sound::IScene {
public:
	SoundScene();
	~SoundScene();
	void release() override;
	
	Emitter* addEmitter() override;
	void remove(sound::IEmitter* emitter) override;
	
	void clear() override;
	
	void setListener(sound::IListener* listener) override;
	
	sound::IListener* getListener() const override;

	void update(float deltaTime) override;
	
private:
	std::unordered_set<Emitter*> emitters;
	
	Listener* activeListener;
};
