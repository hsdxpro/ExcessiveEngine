#pragma once

// namespace
namespace sound {

class IScene;
class ISoundData;
class IListener;

class IEngine
{
public:
	// interface
	virtual void Release() = 0;

	///////////////////////////////////////
	// creators
	
	/// Create a new scene.
	virtual IScene* CreateScene() = 0;
	
	/// Create a new sound resource.
	virtual ISoundData* CreateSoundData() = 0;
	
	/// Create a new listener.
	virtual IListener* CreateListener() = 0;
	
	///////////////////////////////////////
	// states

	virtual void SetScene(IScene* scene) = 0;

	/// Set the volume. Will affect every sound that comes from this sound engine.
	/// volume should be a positive real number.
	/// If volume is 0 the engine is muted,
	/// if it's 1 than the engine is at maximum unampified volume (100%).
	virtual void SetMasterVolume(float volume) = 0;
	
	virtual float GetMasterVolume() const = 0;
	
	// interact
	virtual void Update(float deltaTime) = 0;
};


} // namespace
