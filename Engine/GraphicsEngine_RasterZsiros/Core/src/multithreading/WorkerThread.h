////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	A worker thread object that runs arbitrary functors on another thread.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <queue>
#include <list>
#include <atomic>
#include "spinlock.h"

struct TaskBundleBase;

////////////////////////////////////////////////////////////////////////////////
// Worker thread class
class cWorkerThread {
public:
	//unsigned long long times[10];

	typedef std::promise<TaskBundleBase*> PromiseT;

	cWorkerThread();
	cWorkerThread(const cWorkerThread&) = delete;
	~cWorkerThread();

	cWorkerThread& operator=(const cWorkerThread&) = delete;

	template <class Func, class... Args>
	auto Enqueue(const Func& f, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type>;
private:
	std::queue<PromiseT, std::deque<PromiseT>> execQueue;
	spinlock queueLock;
	std::thread worker;
};


////////////////////////////////////////////////////////////////////////////////
// Enqueue callables on worker

struct TaskBundleBase {
	virtual ~TaskBundleBase() {}
	virtual bool operator()() {
		return false;
	}
};

template <class Func, class... Args>
struct TaskBundle : public TaskBundleBase {
	// return type of Func
	typedef typename std::result_of<Func(Args...)>::type ReturnT;
	// ctor
	TaskBundle(const Func& f, Args&&... args) : func(std::bind(f, std::forward(args)...)) {
	}
	TaskBundle(const TaskBundle& other) = delete;
	TaskBundle(TaskBundle&& other) {
		func = std::move(other.func);
		prms = std::move(other.prms);
	}
	// execute
	template <class T>
	inline void _Execute() {
		prms.set_value(func());
	}
	template <>
	inline void _Execute<void>() {
		func();
		prms.set_value();
	}
	bool operator()() override {
		_Execute<ReturnT>();
		return true;
	}

	// promises...
	std::function<ReturnT()> func;
	std::promise<ReturnT> prms;
};


template <class Func, class... Args>
auto cWorkerThread::Enqueue(const Func& f, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type> {
	using ReturnT = typename std::result_of<Func(Args...)>::type;

	//times[0] = __rdtsc();

	// lock queue
	queueLock.lock();
	//times[1] = __rdtsc();

	auto& backPromise = execQueue.back();
	//times[2] = __rdtsc();

	auto* bundle = new TaskBundle<Func, Args...>(f, args...); // high cost ~7000 cycle(not because of new)
	//times[3] = __rdtsc();

	auto retFuture = bundle->prms.get_future();
	//times[4] = __rdtsc();

	backPromise.set_value(bundle); // high cost ~7000 cycle
	//times[5] = __rdtsc();

	// push the next dummy
	execQueue.push(PromiseT()); // high cost ~7000 cycle
	//times[6] = __rdtsc();

	// unlock queue
	queueLock.unlock();
	//times[7] = __rdtsc();
	/*
	for (int i = 6; i >= 0; --i) {
		times[i + 1] -= times[i];
	}
	*/
	return retFuture;
}