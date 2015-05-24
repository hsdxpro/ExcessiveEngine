// Windows implementation of high resolution timer
#pragma once
#include "..\Common\src\ITimer.h"
#include "..\Common\src\BasicTypes.h"
#include <chrono>

class Timer : public ITimer
{
	void start() override;
	void reset() override;

	double getSecondsPassed() override;

private:
	std::chrono::system_clock::time_point startTime;
};