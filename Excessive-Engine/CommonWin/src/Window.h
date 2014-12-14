// Windows O.S window
#pragma once
#include "..\Common\src\IWindow.h"

#include "SFML\Window\Window.hpp"
#include "SFML\Window\Event.hpp"

class Window : public IWindow
{
public:
	Window(const rWindow& d);

	bool popEvent(rWindowEvent* evt_out) override;

	void close() override;

	void displayClientRect() override;

	// getters
	bool isOpen() const override;

	u16 getClientW() const override;
	u16 getClientH() const override;

	float getClientAspectRatio() const override;

	void setText(const wchar_t* text) override;

protected:
	sf::Window w;
	sf::Event evt;
};