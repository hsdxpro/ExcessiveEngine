#include "LogStream.hpp"
#include "Logger.hpp"


#include <iostream>
using std::cout;
using std::endl;
volatile int calldepth = 0;
std::mutex debugStackMtx;
void BeginCall(const char* function) {	
	debugStackMtx.lock();
	int d = calldepth++;
	for (int i = 0; i < d; i++)
		cout << "  ";
	cout << function << endl;
	debugStackMtx.unlock();
}
void EndCall() {
	calldepth--;
}
void Lock(const char* mtx) {
	debugStackMtx.lock();
	for (int i = 0; i < calldepth; i++)
		cout << "  lock ";
	cout << mtx << endl;
	debugStackMtx.unlock();
}
void Unlock(const char* mtx) {
	debugStackMtx.lock();
	for (int i = 0; i < calldepth; i++)
		cout << "  unlock ";
	cout << mtx << endl;
	debugStackMtx.unlock();
}



namespace exc {


LogStream::LogStream(Logger* parent, uint64_t streamId) {
	BeginCall("LogStream()");

	this->parent = parent;
	this->streamId = streamId;

	EndCall();
}

LogStream::LogStream() {
	parent = nullptr;
	streamId = 0;
}

LogStream::LogStream(LogStream&& rhs) {
	BeginCall("LogStream(move)");

	parent = rhs.parent;
	streamId = rhs.streamId;

	rhs.mtx.lock();
	rhs.parent = nullptr;
	rhs.streamId = 0;
	rhs.mtx.unlock();

	if (parent != nullptr) {
		Lock("logger.mtx");

		std::lock_guard<std::recursive_mutex> lk(LogStreamInterface::GetLock(*parent));
		eventBuffer = std::move(rhs.eventBuffer);
		LogStreamInterface::UpdateInstance(*parent, streamId, this);

		Unlock("logger.mtx");
	}

	EndCall();
}

LogStream::~LogStream() {
	BeginCall("~LogStream");

	if (parent) {
		LogStreamInterface::CloseStream(*parent, *this);
	}

	EndCall();
}

LogStream& LogStream::operator=(LogStream&& rhs) {
	BeginCall("LogStream::operator=(move)");
	Lock("logstream.mtx");
	std::lock_guard<std::recursive_mutex> lk(mtx);

	// destruct phase
	if (parent) {
		LogStreamInterface::CloseStream(*parent, *this);
	}

	// construct phase
	parent = rhs.parent;
	streamId = rhs.streamId;


	Lock("logger.rhs.mtx");
	rhs.mtx.lock();
	rhs.parent = nullptr;
	rhs.streamId = 0;
	rhs.mtx.unlock();
	Unlock("logger.rhs.mtx");


	if (parent != nullptr) {
		Lock("logger.mtx");

		std::lock_guard<std::recursive_mutex> lk(LogStreamInterface::GetLock(*parent));
		eventBuffer = std::move(rhs.eventBuffer);
		LogStreamInterface::UpdateInstance(*parent, streamId, this);

		Unlock("logger.mtx");
	}

	Unlock("logstream.mtx");
	EndCall();
	return *this;
}


uint64_t LogStream::GetStreamId() const {
	return streamId;
}

EventBuffer LogStream::PullFlush() {
	Lock("logstream.mtx");
	std::lock_guard<std::recursive_mutex> lk(mtx);
	EventBuffer tmp = std::move(eventBuffer);
	eventBuffer.clear();
	Unlock("logstream.mtx");
	return tmp;
}


void LogStream::Event(exc::Event e, eEventDisplayMode displayMode) {
	Lock("logstream.mtx");
	std::lock_guard<std::recursive_mutex> lk(mtx);
	if (parent == nullptr) {
		return;
	}
	eventBuffer.push_back({ std::chrono::high_resolution_clock::now(), e });
	Flush();
	Unlock("logstream.mtx");
}

void LogStream::Flush() {
	Lock("logstream.mtx");
	std::lock_guard<std::recursive_mutex> lk(mtx);
	LogStreamInterface::CommitEvents(*parent, *this, std::move(eventBuffer));
	Unlock("logstream.mtx");
}

Logger* LogStream::GetParent() const {
	return parent;
}


} // namespace exc