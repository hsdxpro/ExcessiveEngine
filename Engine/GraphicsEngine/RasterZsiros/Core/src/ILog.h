// ILog.h By Zsíroskenyér Team 2013.10.22 11:33
// interface for error reporting, logging system
#pragma once

#include "zsString.h"

class ILog {
public:
	virtual void MsgBox(const zsString& str) = 0;
	virtual void Log(const zsString& str) = 0;

	static ILog* GetInstance();
	static ILog* instance;
};