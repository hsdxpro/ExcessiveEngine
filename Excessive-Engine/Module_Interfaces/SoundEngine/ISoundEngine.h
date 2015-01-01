#pragma once

// namespace
namespace sound {

class ISoundEngine
{
public:
	// interface
	virtual void release() = 0;

	// interact
	virtual void update() = 0;
};


} // namespace