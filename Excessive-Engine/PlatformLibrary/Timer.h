#ifdef EXC_WINDOWS
#include "Windows/Timer_win.h"
#elif EXC_LINUX
#include "Linux/Timer_linux.h"
#elif ECX_OSX
#include "Linux/Timer_osx.h"
#endif