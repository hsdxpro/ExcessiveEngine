#include "Log.h"

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void cLog::MsgBox(const zsString& str) {
	MessageBoxW(nullptr, str.c_str(), L"Message", MB_OK | MB_ICONWARNING);
}

void cLog::Log(const zsString& str) {
	std::wcout<<str<<std::endl;
}
