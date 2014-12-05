// Windows O.S window
#pragma once
#include "..\Common\src\IWindow.h"

class Window : public IWindow
{
public:
	Window(const IWindow::rDesc& d);

	bool popEvent(IWindow::rEvent* evt_out);

	void close();

	void displayClientRect();

	// getters
	bool isOpen();
};