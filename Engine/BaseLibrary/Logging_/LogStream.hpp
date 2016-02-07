#pragma once

#include "Event.hpp"
#include "EventEntry.hpp"

#include <cstdint>
#include <deque>
#include <chrono>
#include <mutex>


extern volatile int calldepth;


namespace exc {


class Logger;
class LogPipe;

enum class eEventDisplayMode {
	DONT_DISPLAY,
	STDOUT,
	STDERR,
};


class LogStream {
	friend class LoggerInterface;
private:
	LogStream(std::shared_ptr<LogPipe> pipe);
public:
	LogStream();
	LogStream(const LogStream&) = delete;
	LogStream(LogStream&&);
	~LogStream();

	LogStream& operator=(const LogStream&) = delete;
	LogStream& operator=(LogStream&&);

	void Event(const exc::Event& e, eEventDisplayMode displayMode = eEventDisplayMode::DONT_DISPLAY);
	void Event(exc::Event&& e, eEventDisplayMode displayMode = eEventDisplayMode::DONT_DISPLAY);
private:
	std::shared_ptr<LogPipe> pipe;
	std::recursive_mutex mtx;
};


class LoggerInterface {
	friend class Logger;
private:
	inline static LogStream Construct(std::shared_ptr<LogPipe> pipe) {
		return LogStream(pipe);
	}
};



} // namespace exc