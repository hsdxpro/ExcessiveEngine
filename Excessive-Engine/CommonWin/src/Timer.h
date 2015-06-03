// Windows implementation of high resolution timer
#pragma once
#include "..\Common\src\ITimer.h"
#include "..\Common\src\BasicTypes.h"
#include <chrono>

class Timer : public ITimer
{
public:
	Timer();

	void start() override;
	void reset() override;

	double getSecondsPassed() override;
	size_t getMicroSecondsPassed() override;

private:
	// std::chrono::system_clock::time_point startTime;
	unsigned long long frequency;
	unsigned long long startTime;
};