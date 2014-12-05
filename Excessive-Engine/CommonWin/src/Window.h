// Windows O.S window
#pragma once
#include "..\Common\src\IWindow.h"

#include "SFML\Window\Window.hpp"
#include "SFML\Window\Event.hpp"

class Window : public IWindow
{
public:
	Window(const IWindow::rDesc& d);

	bool popEvent(IWindow::rEvent* evt_out);

	void close();

	void displayClientRect();

	// getters
	bool isOpen();

protected:
	sf::Window w;
	sf::Event evt;
};