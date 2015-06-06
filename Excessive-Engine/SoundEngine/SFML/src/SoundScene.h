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
	//Get the transformation that moves an Emitter from the current listener coordinate system to the corresponding SFML coordinate system
	mm::mat4 getSFMLViewTransform() const;

	std::unordered_set<Emitter*> emitters;
	
	Listener* activeListener;

	static const mm::vec3 SFMLforwards;
	static const mm::vec3 SFMLupwards;
};
