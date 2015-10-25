
#include "../Sys.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xlocbuf>
#include <codecvt>

#include <iostream>

Sys::DLLHandle Sys::LoadDLL(const wchar_t* path) 
{
	auto value = LoadLibraryW(path);
	return value;
}

bool Sys::UnLoadDLL(DLLHandle h) 
{
	return FreeLibrary((HMODULE)h) ? true : false;
}

void Sys::MsgBox(const std::wstring& msg)
{
	MessageBoxW(0, msg.c_str(), L"", MB_OK);

}

void Sys::SetCursorPos(const mm::uvec2& pos)
{
	::SetCursorPos(pos.x, pos.y);
}

void* Sys::GetDLLProcAddress(DLLHandle h, const std::string& procName) 
{
	return (void*)GetProcAddress((HMODULE)h, procName.c_str());
}

std::wstring Sys::GetExeDirW()
{
	// TODO make define or constant somewhere (128)
	wchar_t path[128];
	GetModuleFileNameW(0, path, 128);

	// Preserve only directory
	for (int i = 127; i > 0; i--)
		if (path[i] == '\\' && i < 127)
		{
			path[i + 1] = 0;
			break;
		}

	// TODO: lassít, de legalább szép replace '\\' with '/'
	int idx = 0;
	while (path[idx] != '\0') {
		if (path[idx] == '\\')
			path[idx] = '/';
		idx++;
	}

	return path;
}

std::string	Sys::GetExeDir()
{
	// TODO make define or constant somewhere (128)
	char path[256];
	memset(path, 0, sizeof(path));
	GetModuleFileNameA(0, path, 256);

	size_t i = 255;
	while (i > 0 && path[i] != '/' && path[i] != '\\')
		i--;

	path[i + 1] = 0;

	size_t idx = 0;
	while (path[idx] != '\0') 
	{
		if (path[idx] == '\\')
			path[idx] = '/';

		idx++;
	}

	return path;
}

mm::ivec2 Sys::GetCursorPos() 
{
	POINT p; ::GetCursorPos(&p);
	return mm::ivec2(p.x, p.y);
}

mm::ivec2 Sys::GetScreenSize()
{
	return{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}

std::string GetAssetsPath()
{
	return EXCESSIVE_ASSET_DIR;
	//return Sys::GetExeDir() + "../../Assets/";
}