#pragma once
#include "..\INetworkEngine.h"

struct rNetworkEngine {

};

class NetworkEngineBoost : public INetworkEngine
{
public:
	// ctor, dtor, release
	NetworkEngineBoost(const rNetworkEngine& d);
	~NetworkEngineBoost();

	void Release() override;

	// interact
	void Update(float deltaTime) override;

private:
};

