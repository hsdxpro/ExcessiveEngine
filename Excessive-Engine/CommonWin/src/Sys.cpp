
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

std::wstring Sys::getWorkDir() {
	// TODO make define or constant somewhere (128)
	wchar_t path[128];
	GetModuleFileNameW(0, path, 128);

	for (int i = 127; i > 0; i--)
		if (path[i] == '\\' && i < 127)
		{
			path[i + 1] = 0;
			break;
		}	

	return path;
}

const mm::ivec2& Sys::getMousePos() {
	POINT p; GetCursorPos(&p);
	return mm::ivec2(p.x, p.y);
}