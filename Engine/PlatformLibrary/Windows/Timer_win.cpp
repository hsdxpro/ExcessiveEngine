#include "Timer_win.h"

#include <cassert>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

static unsigned long long frequency;

Timer::Timer() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	assert(frequency > 0);
}

void Timer::Start()
{
	//startTime = std::chrono::high_resolution_clock::now();

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
}

void Timer::Reset()
{
	Start();
}

double Timer::GetSecondsPassed()
{	
	// auto currTime = std::chrono::high_resolution_clock::now();
	// return (double)std::chrono::duration_cast<std::chrono::microseconds>(currTime - startTime).count() / 1.0e6;

	unsigned long long currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	return (double)(currentTime - startTime) / (double)frequency;
}

double Timer::GetMicroSecondsPassed()
{
	// auto currTime = std::chrono::high_resolution_clock::now();
	// return std::chrono::duration_cast<std::chrono::microseconds>(currTime - startTime).count();

	unsigned long long currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	return (double)(currentTime - startTime) / (double)frequency * 1e+6;
}