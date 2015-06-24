#pragma once

// namespace
namespace sound {

class IEmitter;
class IListener;

class IScene {
public:
	virtual void Release() = 0;
	
	virtual IEmitter* AddEmitter() = 0;
	virtual void Remove(IEmitter* emitter) = 0;
	
	virtual void clear() = 0;
	
	virtual void SetListener(IListener* listener) = 0;
	
	virtual IListener* GetListener() const = 0;
};


} // namespace
