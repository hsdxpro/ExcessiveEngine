// Interface for typical O.S Window
#pragma once

#include "BasicTypes.h"
#include "ISys.h"

class IWindow
{
public:
	struct rDesc
	{
		u16 w;
		u16 h;
		wchar_t* capText; // TMP, REMOVE THAT
	};

	enum class eMsg
	{
		CLOSED,
		KEY_PRESSED,
	};

	struct rEvent
	{
		eMsg msg;
		ISys::eKey key;
		//key
	};

public:
	// Window events
	virtual bool PopEvent(rEvent* evt_out) = 0;

	virtual void close() = 0;

	virtual void displayClientRect() = 0;

	// getters
	virtual bool isOpen() = 0;
protected:
};