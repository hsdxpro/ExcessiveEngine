// Windows implementation of high resolution timer
#pragma once
#include "..\..\SupportLibrary\BasicTypes.h"
#include <chrono>

class Timer
{
public:
	Timer();

	void Start();
	void Reset();

	double GetSecondsPassed();
	double GetMicroSecondsPassed();

private:
	std::chrono::steady_clock::time_point startTime;
	//unsigned long long startTime;
};