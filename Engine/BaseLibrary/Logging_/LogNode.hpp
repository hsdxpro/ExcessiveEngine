#pragma once

#include <shared_mutex>
#include <atomic>
#include <memory>
#include <vector>
#include <chrono>

namespace exc {


class LogNode {
	friend class LogPipe;
private:
	struct PipeInfo {
		std::shared_ptr<LogPipe> pipe;
		std::string name;
	};
public:
	LogNode();
	~LogNode();

	void Flush();
	std::shared_ptr<LogPipe> CreatePipe(const std::string& name);
	void SetOutputStream(std::ostream* outputStream);
private:
	void NotifyNewEvent();
	void NotifyClose(LogPipe* pipe);

	std::vector<PipeInfo> pipes;
	std::shared_timed_mutex mtx;
	std::atomic_bool prohibitPipes;
	std::atomic_ptrdiff_t pendingEvents;

	std::ostream* outputStream;
	std::chrono::high_resolution_clock::time_point startTime;

	static constexpr ptrdiff_t flushThreshold = 1000;
};


} // namespace exc