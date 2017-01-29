////////////////////////////////////////////////////////////////////////////////
//	The interface for working with dynamic libraries.
//	To be implemented in OS specific static libraries.
////////////////////////////////////////////////////////////////////////////////


#pragma once 

typedef unsigned long long DllHandle;


DllHandle LoadDynamicLibrary(const wchar_t* fileName);
DllHandle LoadDynamicLibrary(const char* fileName);
void UnloadDynamicLibrary(DllHandle module);

void* GetFunctionAddress(DllHandle module, const char* funcName);
