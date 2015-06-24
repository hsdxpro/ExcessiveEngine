// Interface class for high resolution timing
#pragma once

class ITimer
{
public:
	// Start timing
	virtual void Start() = 0;

	// restart timing
	virtual void Reset() = 0;

	// Get seconds passed since start() or Reset()
	virtual double GetSecondsPassed() = 0;

	// Get micro seconds passed since start() or Reset()
	virtual double GetMicroSecondsPassed() = 0;
};