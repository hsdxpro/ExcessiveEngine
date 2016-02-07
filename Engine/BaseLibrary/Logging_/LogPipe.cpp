#include "LogPipe.hpp"
#include "LogNode.hpp"

#include <thread>

namespace exc {


LogPipe::LogPipe(LogNode* node) {
	this->node = node;
}

void LogPipe::PutEvent(const Event& evt) {
	// Spin until we're allowed to even try to lock.
	// This is to avoid starvation of LogNode.
	while (node->prohibitPipes) {
		std::this_thread::yield();
	}

	// Shared lock allows other pipes to do their stuff concurrently.
	node->mtx.lock_shared();



	node->mtx.unlock_shared();
}

void LogPipe::PutEvent(Event&& evt) {

}


} // namespace exc