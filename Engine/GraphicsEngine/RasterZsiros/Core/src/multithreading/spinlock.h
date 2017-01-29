////////////////////////////////////////////////////////////////////////////////
//	File:   spinlock.h
//	Author: Péter Kardos
////////////////////////////////////////////////////////////////////////////////
//	Mutex object using spinlocks.
//	
//	Works the same as the STL std::mutex. See cppreference.com for documentation.
//	IMPORTANT: this spinlock does NOT care about a threads's identity. It can be
//	unlocked from a thread other than the locking thread. Be careful!
////////////////////////////////////////////////////////////////////////////////

#ifndef SPINLOCK_H
#define	SPINLOCK_H

#include <atomic>


class spinlock {
	public:
		spinlock();
		
		void lock();
		bool try_lock();
		void unlock();
	private:	
		std::atomic<bool> lock_var;	
};


#endif	/* SPINLOCK_H */

