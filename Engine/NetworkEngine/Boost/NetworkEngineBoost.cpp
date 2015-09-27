#include "NetworkEngineBoost.h"

////////////////////////////////////////////////////////////////////////////////
// Export Create function
//
////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C"
EXPORT INetworkEngine* CreateNetworkEngine(const rNetworkEngine& d) {
	return new NetworkEngineBoost(d);
}

NetworkEngineBoost::NetworkEngineBoost(const rNetworkEngine& d) {
}

NetworkEngineBoost::~NetworkEngineBoost() {
}


void NetworkEngineBoost::Release() {
	delete this;
}

void NetworkEngineBoost::Update(float deltaTime) {
	
}