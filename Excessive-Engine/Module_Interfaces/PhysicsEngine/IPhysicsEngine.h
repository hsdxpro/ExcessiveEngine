#pragma once

// namespace
namespace physics {

class IPhysicsEngine
{
public:
	// interface
	virtual void release() = 0;

	// interact
	virtual void update(float deltaTime) = 0;
};


} // namespace