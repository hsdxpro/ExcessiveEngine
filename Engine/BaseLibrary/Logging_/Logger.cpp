#include "Logger.hpp"
#include "LogStream.hpp"

#include <cassert>

#include <iostream>
using std::cout;
using std::endl;

namespace exc {

Logger::Logger() {
	nextStreamId = 0;
	numPendingEvents = 0;
	startTime = std::chrono::steady_clock::now();
}

Logger::~Logger() {
	FlushAllStreams();
	FlushToFile();
}

bool Logger::OpenFile(const std::string& path) {
	Close();
	outputStream.open(path, std::ios::out | std::ios::trunc);
	return outputStream.is_open();
}


void Logger::Close() {
	outputStream.close();
}


LogStream Logger::CreateLogStream(const std::string& name) {
	LogStream s = LoggerInterface::Construct(this, nextStreamId);
	streamRecords.insert({ nextStreamId, {EventBuffer(), name, &s} });
	++nextStreamId;
	return std::move(s);
}


void Logger::CloseStream(const LogStream& stream) {
	std::lock_guard<std::recursive_mutex> lkg(mtx);

	// Flush content.
	FlushAllStreams();
	FlushToFile();

	// Remove this stream from records.
	if (this == stream.GetParent()) {
		streamRecords.erase(LoggerInterface::GetStreamId(stream));
	}
}


void Logger::UpdateInstance(uint64_t streamId, LogStream* instance) {
	if (instance->GetParent() != this) {
		return;
	}
	std::lock_guard<std::recursive_mutex> lk(mtx);
	auto recordIt = streamRecords.find(streamId);
	assert(recordIt != streamRecords.end());
	recordIt->second.instance = instance;
}


void Logger::CommitEvents(const LogStream& commiter, EventBuffer events) {
	std::lock_guard<std::recursive_mutex> lkg(mtx);

	// check parameters, get streamId buffer
	if (commiter.GetParent() != this) {
		return;
	}
	uint64_t streamId = LoggerInterface::GetStreamId(commiter);
	auto it = streamRecords.find(streamId);
	assert(it != streamRecords.end());
	EventBuffer& myBuffer = it->second.buffer;

	// copy events to buffer
	for (auto& e : events) {
		myBuffer.push_back(std::move(e));
		numPendingEvents++;
	}

	// flush to logfile if necessary
	if (numPendingEvents >= flushThreshold) {
		FlushAllStreams();
		FlushToFile();
	}
}


void Logger::FlushAllStreams() {
	std::lock_guard<std::recursive_mutex> lkg(mtx);
	for (auto& record : streamRecords) {
		EventBuffer buffer = LoggerInterface::PullFlush(*record.second.instance);
		EventBuffer& myBuffer = record.second.buffer;

		// copy events to buffer
		for (auto& e : buffer) {
			myBuffer.push_back(std::move(e));
			numPendingEvents++;
		}
	}
}

void Logger::FlushToFile() {
	std::lock_guard<std::recursive_mutex> lkg(mtx);

	while (true) {
		// select buffer with oldest timestamp at front
		StreamRecord* next = nullptr;
		std::chrono::high_resolution_clock::time_point nextTimeStamp = std::chrono::high_resolution_clock::time_point::max();
		for (auto& v : streamRecords) {
			StreamRecord& record = v.second;
			if (!record.buffer.empty()) {
				if (record.buffer[0].timestamp < nextTimeStamp) {
					next = &record;
					nextTimeStamp = record.buffer[0].timestamp;
				}
			}
		}

		// write to log stream the first item of that buffer
		if (next != nullptr) {
			Event& nextEvent = next->buffer[0].event;
			if (outputStream.is_open()) {
				outputStream
					<< "[" << std::chrono::duration_cast<std::chrono::microseconds>(nextTimeStamp - startTime).count() / 1.e6 << "]"
					<< "[" << next->name << "] "
					<< nextEvent.GetMessage() << "\n";
				for (size_t i = 0; i < nextEvent.GetNumParameters(); i++) {
					outputStream << "   " << nextEvent[i].name << " = " << nextEvent[i].ToString() << "\n";
				}
			}

			// pop event from buffer
			next->buffer.pop_front();
			numPendingEvents--;
		}
		else {
			outputStream << "--- FLUSH ---" << std::endl;
			break; // no record left
		}
	}
}



} // namespace exc