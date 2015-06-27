
#include "..\Common\src\Sys.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

Sys::DLLHandle Sys::loadDLL(const std::string& filePath) {
	return LoadLibrary(filePath.c_str());
}

bool Sys::unloadDLL(DLLHandle h) {
	return FreeLibrary((HMODULE)h) ? true : false;
}

void* Sys::getDllProcAddress(DLLHandle h, const std::string& procName) {
	return (void*)GetProcAddress((HMODULE)h, procName.c_str());
}