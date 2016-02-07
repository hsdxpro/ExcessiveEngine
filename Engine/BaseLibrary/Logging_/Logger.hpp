#pragma once

#include "LogStream.hpp"
#include "EventEntry.hpp"

#include <string>
#include <ostream>
#include <fstream>
#include <mutex>
#include <cstdint>
#include <map>

extern volatile int calldepth;

namespace exc {


class Logger {
	friend class LogStreamInterface;
	struct StreamRecord {
		EventBuffer buffer;
		std::string name;
		LogStream* instance;
	};
public:
	Logger();
	~Logger();

	bool OpenFile(const std::string& path);
	void Close();

	LogStream CreateLogStream(const std::string& name);
private:
	void CommitEvents(const LogStream& commiter, EventBuffer events);
	void CloseStream(const LogStream& stream);
	void UpdateInstance(uint64_t streamId, LogStream* instance);

	void FlushAllStreams();
	void FlushToFile();
private:
	std::ofstream outputStream;
	std::recursive_mutex mtx;
	uint64_t nextStreamId;
	std::map<uint64_t, StreamRecord> streamRecords;
	size_t numPendingEvents;
	static constexpr size_t flushThreshold = 5;
	std::chrono::steady_clock::time_point startTime;
};




class LogStreamInterface {
	friend class LogStream;
private:
	inline static void CommitEvents(Logger& logger, const LogStream& commiter, EventBuffer events) {
		logger.CommitEvents(commiter, events);
	}

	inline static void CloseStream(Logger& logger, const LogStream& me) {
		logger.CloseStream(me);
	}

	inline static void UpdateInstance(Logger& logger, uint64_t streamId, LogStream* instance) {
		logger.UpdateInstance(streamId, instance);
	}
	
	inline static std::recursive_mutex& GetLock(Logger& logger) {
		return logger.mtx;
	}
};


} // namespace exc