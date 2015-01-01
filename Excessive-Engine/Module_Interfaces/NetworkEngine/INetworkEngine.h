#pragma once

// namespace
namespace network {

class INetworkEngine
{
public:
	// interface
	virtual void release() = 0;

	// interact
	virtual void update() = 0;
};


} // namespace