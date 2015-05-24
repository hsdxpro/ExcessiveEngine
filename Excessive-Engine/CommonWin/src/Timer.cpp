#include "Timer.h"

void Timer::start()
{
	startTime = std::chrono::high_resolution_clock::now();
}

void Timer::reset()
{
	start();
}

double Timer::getSecondsPassed()
{
	auto currTime = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(currTime - startTime).count() / 1.0e6;
}