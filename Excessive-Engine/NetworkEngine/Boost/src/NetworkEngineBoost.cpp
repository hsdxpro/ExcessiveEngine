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
EXPORT network::IEngine* CreateNetworkEngine(const rNetworkEngine& d) {
	return new NetworkEngineBoost(d);
}

NetworkEngineBoost::NetworkEngineBoost(const rNetworkEngine& d) {
}

NetworkEngineBoost::~NetworkEngineBoost() {
}


void NetworkEngineBoost::release() {
	delete this;
}

void NetworkEngineBoost::update(float deltaTime) {
	
}