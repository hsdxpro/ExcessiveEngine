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
	virtual void release() = 0;

	///////////////////////////////////////
	// creators
	
	/// Create a new scene.
	virtual IScene* createScene() = 0;
	
	/// Create a new sound resource.
	virtual ISoundData* createSoundData() = 0;
	
	/// Create a new listener.
	virtual IListener* createListener() = 0;
	
	///////////////////////////////////////
	// states

	/// Set the volume. Will affect every sound that comes from this sound engine.
	/// volume should be a positive real number.
	/// If volume is 0 the engine is muted,
	/// if it's 1 than the engine is at maximum unampified volume (100%).
	virtual void setMasterVolume(float volume) = 0;
	
	virtual float getMasterVolume() const = 0;
	
	// interact
	virtual void update(float deltaTime) = 0;
};


} // namespace
