////////////////////////////////////////////////////////////////////////////////
//	File:   spinlock.cpp
//	Author: Péter Kardos
////////////////////////////////////////////////////////////////////////////////
//	See header.
//
////////////////////////////////////////////////////////////////////////////////


#include "spinlock.h"

	
////////////////////////////////////////////////////////////////////////////////
//	mutex member functions

	
////////////////////////////////////////
// constructor
spinlock::spinlock() {
	lock_var = false;
}

////////////////////////////////////////
// acquire mutex
void spinlock::lock() {
	bool success;
	// loop infinitely
	while (true) {
		// loop until mutex is free
		while (lock_var != false);
		// exchange the value
		success = lock_var.exchange(true);
		// if the value is false, thus the mutex was previously unlocked, we've acquired it
		if (success == false) {
			break;
		}
		// if the value is true, the mutex is still locked by some other thread
	}
}


////////////////////////////////////////
// try to lock mutex
	// Only one attempt, and then returns. Does not 'block' the calling thread.
	// True on successful locking, otherwise false (see cppreference.com).
bool spinlock::try_lock() {
	bool success;
	// one try to lock this
	success = lock_var.exchange(true);
	// previously 'false' means we've just acquired the lock
	if (success == false) {
		return true;
	}
	else {
		return false;
	}
}


////////////////////////////////////////
// release the mutex
	// WARNING: There is no fool protection!
	// It is assumed that the mutex is locked by the calling thread,
	// if you do it otherwise, your application might die.
	// Attempt to unlock an already unlocked mutex from any thread is totally safe. 
void spinlock::unlock() {
	lock_var.store(false);	
}