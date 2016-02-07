#pragma once

#include "EventEntry.hpp"


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
private:
	EventBuffer buffer;
	LogNode* node;
};


} // namespace exc