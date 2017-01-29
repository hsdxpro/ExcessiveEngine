#include "ThreadPool.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// ctor
cThreadPool::cThreadPool() {
	nWorkers = GetNumLogicalCores();
	if (nWorkers == 0) {
		nWorkers = 1;
	}
	workers = new cWorkerThread[nWorkers];
}

cThreadPool::cThreadPool(size_t nThreads) {
	nWorkers = nThreads;
	if (nWorkers == 0) {
		nWorkers = 1;
	}
	workers = new cWorkerThread[nWorkers];
}

cThreadPool::~cThreadPool() {
	delete[] workers;
}



////////////////////////////////////////////////////////////////////////////////
// misc
size_t cThreadPool::GetNumThreads() {
	return nWorkers;
}

size_t cThreadPool::GetNumLogicalCores() {
	return thread::hardware_concurrency();
}