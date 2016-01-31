#pragma once
#include "..\INetworkEngine.h"

struct rNetworkEngine {

};

class NetworkEngineRakNet : public INetworkEngine
{
public:
	NetworkEngineRakNet(const rNetworkEngine& d);
	~NetworkEngineRakNet();

	void Release() override;

	void Update(float deltaTime) override;
private:
};

