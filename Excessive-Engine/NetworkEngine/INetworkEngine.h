#pragma once

class INetworkEngine
{
public:
	// interface
	virtual void Release() = 0;

	// interact
	virtual void Update(float deltaTime) = 0;
};