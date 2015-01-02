#pragma once

// namespace
namespace sound {

class IEngine
{
public:
	// interface
	virtual void release() = 0;

	// interact
	virtual void update(float deltaTime) = 0;
};


} // namespace