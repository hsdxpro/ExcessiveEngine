#pragma once

#include <shared_mutex>
#include <atomic>

namespace exc {


class LogNode {
	friend class LogPipe;
public:



private:
	std::shared_timed_mutex mtx;
	std::atomic_bool prohibitPipes;
};


} // namespace exc