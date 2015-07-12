#pragma once

// namespace
namespace network {

class IEngine
{
public:
	// interface
	virtual void Release() = 0;

	// interact
	virtual void Update(float deltaTime) = 0;
};


} // namespace