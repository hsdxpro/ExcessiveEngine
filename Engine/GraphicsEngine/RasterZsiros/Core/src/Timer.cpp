#include "Timer.h"

cTimer::cTimer() {
	curr = std::chrono::system_clock::now();
}

float cTimer::GetDeltaSeconds() {
	std::chrono::system_clock::time_point tmp = std::chrono::system_clock::now();
	float deltaT = std::chrono::duration_cast<std::chrono::duration<float>>(tmp - curr).count();
	curr = tmp;

	return deltaT;
}

double cTimer::GetDeltaSecondsPrecise() {
	std::chrono::system_clock::time_point tmp = std::chrono::system_clock::now();
	double deltaT = std::chrono::duration_cast<std::chrono::duration<double>>(tmp - curr).count();
	curr = tmp;

	return deltaT;
}

size_t cTimer::GetFps(float deltaSeconds) {
	// add delta micro time each call
	static float collectedSeconds = 0;
	collectedSeconds += deltaSeconds;
	static size_t frameCount = 0;
	frameCount++;

	static size_t lastFrameCount = 30;

	// reach 1 million micro sec, so 1 second passed, return the frame count that passed
	if (collectedSeconds > 1.0f) {
		size_t result = frameCount;
		lastFrameCount = result;
		collectedSeconds = 0;
		frameCount = 0;
		return result;
	}
	else {
		return lastFrameCount;
	}
}