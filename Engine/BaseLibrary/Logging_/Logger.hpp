#pragma once

#include "LogStream.hpp"

#include <string>
#include <ostream>


namespace exc {


class Logger {
	friend class LogStreamInterface;
public:
	Logger();

	bool OpenFile(const std::string& path);
	bool OpenStream(std::ostream& stream);
	void Close();

	LogStream CreateLogStream(const std::string& name);


private:
	template <class EventIter>
	void CommitEvents(const LogStream& commiter, EventIter first, EventIter EventIter);

	void CloseStream(const LogStream& stream);
private:
	
};


class LogStreamInterface {
	friend class LogStream;
private:
	template <class EventIter>
	static void CommitEvents(Logger& logger, const LogStream& commiter, EventIter first, EventIter last) {
		logger.CommitEvents(commiter, first, last);
	}

	static void CloseStream(Logger& logger, const LogStream& me);
};


} // namespace exc