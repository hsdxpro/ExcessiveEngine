#include "Timer.h"
#define _WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>


Timer::Timer() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0) {
		frequency = 100; // at least it won't crash, can't happen on XP or later
	}
}

void Timer::start()
{
	//startTime = std::chrono::high_resolution_clock::now();

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
}

void Timer::reset()
{
	start();
}

double Timer::getSecondsPassed()
{	
	// auto currTime = std::chrono::high_resolution_clock::now();
	// return (double)std::chrono::duration_cast<std::chrono::microseconds>(currTime - startTime).count() / 1.0e6;

	unsigned long long currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	return (double)(currentTime - startTime) / (double)frequency;
}

size_t Timer::getMicroSecondsPassed()
{
	// auto currTime = std::chrono::high_resolution_clock::now();
	// return std::chrono::duration_cast<std::chrono::microseconds>(currTime - startTime).count();

	unsigned long long currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	return (double)(currentTime - startTime) / (double)frequency * 1e+6;
}