// Static class that encapsulates the Operating System.
#pragma once

#include <string>
class Sys
{
public:
	enum class eKey
	{
		ESCAPE,
	};

	typedef void* DLLHandle;

	static DLLHandle	loadDLL(const std::string& filePath);
	static bool			unloadDLL(DLLHandle h);

	static void*		getDllProcAddress(DLLHandle h, const std::string& procName);
};