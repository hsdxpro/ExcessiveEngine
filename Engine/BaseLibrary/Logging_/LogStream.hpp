#pragma once

#include "Event.hpp"

#include <cstdint>
#include <deque>
#include <chrono>

namespace exc {


class Logger;

class LogStream {
	friend class LoggerInterface;
private:
	struct EventEntry {
		std::chrono::high_resolution_clock::time_point timestamp;
		Event e;
	};

private:
	LogStream(Logger* parent, uint64_t streamId);
	uint64_t GetStreamId() const;
public:
	LogStream();
	LogStream(const LogStream&) = delete;
	LogStream(LogStream&&);
	~LogStream();

	void Event(Event e);
	void Flush();

	void GetParent() const;
private:
	Logger* parent;
	uint64_t streamId;
	std::deque<Event> eventBuffer;
};


class LoggerInterface {
	friend class Logger;
private:
	static LogStream Construct(Logger* parent, uint64_t streamId);
	static uint64_t GetStreamId(const LogStream& stream);
};



} // namespace exc