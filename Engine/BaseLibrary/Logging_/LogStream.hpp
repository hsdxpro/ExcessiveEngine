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


enum class eEventDisplayMode {
	DONT_DISPLAY,
	STDOUT,
	STDERR,
};


class LogStream {
	friend class LoggerInterface;
private:
	LogStream(Logger* parent, uint64_t streamId);
public:
	LogStream();
	LogStream(const LogStream&) = delete;
	LogStream(LogStream&&);
	~LogStream();

	LogStream& operator=(const LogStream&) = delete;
	LogStream& operator=(LogStream&&);

	void Event(Event e, eEventDisplayMode displayMode = eEventDisplayMode::DONT_DISPLAY);

	Logger* GetParent() const;
private:
	Logger* parent;
	uint64_t streamId;
	EventBuffer eventBuffer;
	std::recursive_mutex mtx;
};


class LoggerInterface {
	friend class Logger;
private:
	inline static LogStream Construct(Logger* parent, uint64_t streamId) {
		return LogStream(parent, streamId);
	}
};



} // namespace exc