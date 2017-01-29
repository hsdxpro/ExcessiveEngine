#pragma once
#include "SoundEngine/IScene.h"

#include "Emitter.h"
#include "Listener.h"

#include <unordered_set>


class SoundScene : public sound::IScene {
public:
	SoundScene();
	~SoundScene();
	void Release() override;
	
	Emitter* AddEmitter() override;
	void Remove(sound::IEmitter* emitter) override;
	
	void clear() override;
	
	void SetListener(sound::IListener* listener) override;
	
	sound::IListener* GetListener() const override;

	void Update(float deltaTime);
	
private:
	//Get the transformation that moves an Emitter from the current listener coordinate system to the corresponding SFML coordinate system
	mm::mat4 GetSFMLViewTransform() const;

	std::unordered_set<Emitter*> emitters;
	
	Listener* activeListener;

	static const mm::vec3 SFMLforwards;
	static const mm::vec3 SFMLupwards;
};
