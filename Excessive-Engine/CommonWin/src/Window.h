// Windows O.S window
#pragma once
#include "..\Common\src\IWindow.h"

#include "SFML\Window\Window.hpp"
#include "SFML\Window\Event.hpp"

class Window : public IWindow
{
public:
	Window(const rWindow& d);

	bool popEvent(rWindowEvent* evt_out);

	void close();

	void displayClientRect();

	// getters
	bool isOpen() const;

	u16 getClientW() const;
	u16 getClientH() const;

protected:
	sf::Window w;
	sf::Event evt;
};