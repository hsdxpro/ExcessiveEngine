////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	A thread pool that can easily run function on multiple cWorkerThreads.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WorkerThread.h"
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// class

class cThreadPool {
private:
	// helper classes for template bullshit
	template <class ReturnT>
	struct cEnqueue {
		typedef std::future<std::vector<ReturnT>> ExecRetT;
		template <class Func, class... Args>
		static  ExecRetT Exec(cThreadPool& pool, Func f, Args&&... args) {
			// returned result
			auto* retPromise = new std::promise<std::vector<ReturnT>>;
			auto retFuture = retPromise->get_future();

			// launch workers
			auto& workerFutures = *new std::vector<std::future<ReturnT>>;
			for (size_t i = 0; i < pool.nWorkers; i++) {
				workerFutures.push_back(pool.workers[i].Enqueue(std::forward<Func>(f), std::forward<Args>(args)...));
			}
			// launch special function on one thread
			pool.workers[0].Enqueue(
				[retPromise, &workerFutures]() {
				std::vector<ReturnT> workerResults;
				for (auto& fut : workerFutures) {
					fut.wait();
					workerResults.push_back(fut.get());
				}
				retPromise->set_value(workerResults);
				delete retPromise;
				delete &workerFutures;
			});

			return retFuture;
		}
	};
	template <>
	struct cEnqueue<void> {
		typedef std::future<void> ExecRetT;
		template <class Func, class... Args>
		static  ExecRetT Exec(cThreadPool& pool, Func f, Args&&... args) {
			// returned result
			auto* retPromise = new std::promise<void>;
			auto retFuture = retPromise->get_future();

			// launch workers
			auto& workerFutures = *new std::vector<std::future<void>>;
			for (size_t i = 0; i < pool.nWorkers; i++) {
				workerFutures.push_back(pool.workers[i].Enqueue(std::forward<Func>(f), std::forward<Args>(args)...));
			}
			// launch special function on one thread
			pool.workers[0].Enqueue(
				[retPromise, &workerFutures]() {
				for (auto& fut : workerFutures) {
					fut.wait();
				}
				retPromise->set_value();
				delete retPromise;
				delete &workerFutures;
			});

			return retFuture;
		}
	};

	cWorkerThread* workers;
	size_t nWorkers;

public:
	cThreadPool();
	cThreadPool(size_t nThreads);
	~cThreadPool();

	size_t GetNumThreads();
	static size_t GetNumLogicalCores();

	template <class Func, class... Args>
	auto EnqueueForeach(Func f, Args&&... args)
		-> typename cEnqueue<typename std::result_of<Func(Args...)>::type>::ExecRetT;
};


////////////////////////////////////////////////////////////////////////////////
// Good enqueue
template <class Func, class... Args>
auto cThreadPool::EnqueueForeach(Func f, Args&&... args)
-> typename cEnqueue<typename std::result_of<Func(Args...)>::type>::ExecRetT
{
	using ReturnT = typename std::result_of<Func(Args...)>::type;

	return cEnqueue<ReturnT>::Exec(*this, f, std::forward(args)...);
}