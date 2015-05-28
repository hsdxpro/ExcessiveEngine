#pragma once

// namespace
namespace sound {

class IEmitter;
class IListener;

class IScene {
public:
	virtual void release() = 0;
	
	virtual IEmitter* addEmitter() = 0;
	virtual void remove(IEmitter* emitter) = 0;
	
	virtual void clear() = 0;
	
	virtual void setListener(IListener* listener) = 0;
	
	virtual IListener* getListener() const = 0;

	virtual void update(float deltaTime) = 0;
};


} // namespace
