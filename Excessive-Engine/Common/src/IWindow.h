// Interface for typical O.S Window
#pragma once

#include "BasicTypes.h"
#include "Sys.h"

// TMP, REMOVE THAT
#include <string>

class IWindow
{
public:
	struct rDesc {
		u16 clientW;
		u16 clientH;
		std::wstring capText;
	};

	enum class eMsg {
		CLOSED,
		KEY_PRESSED,
	};

	struct rEvent {
		eMsg msg;
		Sys::eKey key;
	};

public:
	virtual void close() = 0;

	virtual bool popEvent(rEvent* evt_out) = 0;

	virtual void displayClientRect() = 0;

	// getters
	virtual bool isOpen() = 0;
};