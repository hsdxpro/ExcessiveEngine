#pragma once

#include "LogStream.hpp"
#include "LogNode.hpp"

#include <string>
#include <ostream>
#include <fstream>
#include <memory>

extern volatile int calldepth;

namespace exc {


class Logger {
public:
	Logger();
	~Logger();

	bool OpenFile(const std::string& path);
	void OpenStream(std::ostream* stream);
	void CloseStream();

	LogStream CreateLogStream(const std::string& name);

	void Flush();
private:
	// do not ever flip the order of the two below!
	// myNode must be destroyed first because it's using outputFile
	std::unique_ptr<std::ofstream> outputFile;
	std::unique_ptr<LogNode> myNode;
};


} // namespace exc