#ifndef assert_h
#define assert_h

#include <signal.h>
#include "logger.h"

#ifndef WIN_BUILD
#define DEBUG_BREAK() raise(SIGTRAP)
#else
#define DEBUG_BREAK() __debugbreak()
#endif

#ifdef DEBUG_BUILD
  #define ASSERT(expr) \
    if( expr ){} \
    else \
    { \
      my::log << my::lock << \
      "Assert failed: " << #expr << my::endl << \
      "File: " << __FILE__ << my::endl << \
      "Line: " << __LINE__ << my::endl << my::unlock; \
      DEBUG_BREAK(); \
    }
#else
  #define ASSERT(expr) //do nothing
#endif

#endif
