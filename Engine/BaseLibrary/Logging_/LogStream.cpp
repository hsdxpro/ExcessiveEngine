#include "LogStream.hpp"
#include "LogPipe.hpp"


namespace exc {


LogStream::LogStream(std::shared_ptr<LogPipe> pipe) {
	this->pipe = pipe;
}

LogStream::LogStream() {
	pipe = nullptr;
}

LogStream::LogStream(LogStream&& rhs) {
	pipe = rhs.pipe;
	rhs.pipe.reset();
}

LogStream::~LogStream() {
	if (pipe) {
		pipe->Close();
	}
}

LogStream& LogStream::operator=(LogStream&& rhs) {
	if (pipe) {
		pipe->Close();
	}

	pipe = rhs.pipe;
	rhs.pipe.reset();

	return *this;
}

void LogStream::Event(const exc::Event& e, eEventDisplayMode displayMode) {
	if (pipe) {
		pipe->PutEvent(e);
	}
}

void LogStream::Event(exc::Event&& e, eEventDisplayMode displayMode) {
	if (pipe) {
		pipe->PutEvent(std::move(e));
	}
}



} // namespace exc