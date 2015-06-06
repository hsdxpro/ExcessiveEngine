// Interface class for high resolution timing
#pragma once

class ITimer
{
public:
	// Start timing
	virtual void start() = 0;

	// restart timing
	virtual void reset() = 0;

	// get seconds passed since start() or reset()
	virtual double getSecondsPassed() = 0;

	// get micro seconds passed since start() or reset()
	virtual double getMicroSecondsPassed() = 0;
};