
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

std::string	Sys::getWorkDir() {
	//std::string path;
	//path.reserve(128); // TODO make define or constant somewhere
	char path[128];
	GetModuleFileName(0, path, 128);
	//u32 pos = path.find(":");

	for (int i = 127; i > 0; i--)
		if (path[i] == '\\' && i < 127)
		{
			path[i + 1] = 0;
			break;
		}	

	std::string result = path;
	return result;
}