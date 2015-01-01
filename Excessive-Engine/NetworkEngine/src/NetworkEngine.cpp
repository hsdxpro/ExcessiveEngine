#include "NetworkEngine.h"

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
EXPORT network::INetworkEngine* CreateNetworkEngine(const rNetworkEngine& d) {
	return new NetworkEngine(d);
}

NetworkEngine::NetworkEngine(const rNetworkEngine& d) {
}

NetworkEngine::~NetworkEngine() {
}


void NetworkEngine::release() {
	delete this;
}

void NetworkEngine::update() {
	
}