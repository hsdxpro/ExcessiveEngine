#include "ILog.h"

#include "../../CommonOS/src/Log.h"
#include "common.h"

ILog* ILog::instance = nullptr;

ILog* ILog::GetInstance() {
	if(ILog::instance == nullptr)
		ILog::instance = new cLog();
	return ILog::instance;
}