////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	A worker thread object that runs arbitrary functors on another thread.
//
////////////////////////////////////////////////////////////////////////////////

#include "WorkerThread.h"

using namespace std;

cWorkerThread::cWorkerThread() {
	// push the first dummy promise
	execQueue.push(PromiseT());
	
	// launch the worker async thread
	worker = thread(
	[this]() {
		bool isContinue = true;
		do {
			// get future
			queueLock.lock();
			auto& execPromise = execQueue.front();
			auto execFuture = execPromise.get_future();
			queueLock.unlock();

			// wait future
			execFuture.wait();

			// execute task
			auto functor = execFuture.get();
			isContinue = (*functor)();

			// pop and drop task
			queueLock.lock();
			delete functor;
			execQueue.pop();
			queueLock.unlock();
		} while (isContinue);
	});
}

cWorkerThread::~cWorkerThread() {
	// set the last promise to exit signal
	auto& frontPromise = execQueue.back();
	frontPromise.set_value(new TaskBundleBase());

	// join thread
	worker.join();
}