// Timer.h By Zsíroskenyér Team 2013.10.27 20:51
#pragma once

#include <chrono>

class cTimer {
public:
	cTimer();

	// Get delta seconds since last call
	float GetDeltaSeconds();

	// Get delta seconds since last call (precise)
	double GetDeltaSecondsPrecise();

	// Get Frames per second
	static size_t GetFps(float deltaSeconds);
protected:
	std::chrono::system_clock::time_point curr;
};