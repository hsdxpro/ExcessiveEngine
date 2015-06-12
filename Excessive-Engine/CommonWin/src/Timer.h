// Windows implementation of high resolution timer
#pragma once
#include "..\Common\src\ITimer.h"
#include "..\Common\src\BasicTypes.h"
#include <chrono>

class Timer : public ITimer
{
public:
	Timer();

	void Start() override;
	void Reset() override;

	double GetSecondsPassed() override;
	double GetMicroSecondsPassed() override;

private:
	// std::chrono::system_clock::time_point startTime;
	unsigned long long startTime;
};