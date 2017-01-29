// common.h By Zsíroskenyér team 2013.10.10 1:51
#pragma once

#include <cassert>
#include "ILog.h"
#include <memory.h>

#if (defined(_DEBUG) || defined(DEBUG))
#define ASSERT(condition) assert(condition)
#define ASSERT_MSG(condition, errorMsg) \
	if (condition != true) { \
		ILog::GetInstance()->MsgBox(errorMsg); \
		assert(condition); \
	}
#else
#define ASSERT_MSG(x, y) {}
#define ASSERT(x) {}
#endif


#define SAFE_DELETE(x) if(x != nullptr) {delete x; x = nullptr;}
#define SAFE_DELETE_ARRAY(x) if(x != nullptr) {delete[] x; x = nullptr;}
#define SAFE_FREE(x) if(x != nullptr) { free(x); x = nullptr;}
#define SAFE_RELEASE(x) if(x != nullptr) { x->Release(); x = nullptr;}

#define ZS_NUMERIC_MAX(x) std::numeric_limits<x>::max()
#define ZS_NUMERIC_MIN(x) std::numeric_limits<x>::min()

#define ZS_PI			3.14159265358979f
#define ZS_PI2			6.28318530717958f
#define ZS_PIDIV2		1.57079632679489f
#define ZS_PIDIV4		0.78539816339744f

void Realloc(void*& mem, size_t oldSize, size_t newSize);
