#pragma once
#include "../../Interfaces/IEngine.h"

struct rNetworkEngine {

};

class NetworkEngineBoost : public network::IEngine
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

