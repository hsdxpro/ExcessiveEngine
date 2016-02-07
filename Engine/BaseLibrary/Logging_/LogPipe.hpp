#pragma once

#include "EventEntry.hpp"

#include <BaseLibrary/Utility_/Spinlock.hpp>
#include <mutex>


namespace exc {

class LogPipe {
	friend class LogNode;
private:
	LogPipe(LogNode* node);
public:
	LogPipe(const LogPipe&) = delete;
	LogPipe& operator=(const LogPipe&) = delete;

	void PutEvent(const Event& evt);
	void PutEvent(Event&& evt);
	void Close();
private:
	EventBuffer buffer;
	LogNode* node;
	std::mutex pipeLock;
};


} // namespace exc