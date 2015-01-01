#pragma once
#include "../Module_Interfaces/NetworkEngine/INetworkEngine.h"

struct rNetworkEngine {

};

class NetworkEngine : public network::INetworkEngine
{
public:
	// ctor, dtor, release
	NetworkEngine(const rNetworkEngine& d);
	~NetworkEngine();

	void release() override;

	// interact
	void update() override;

private:
};

