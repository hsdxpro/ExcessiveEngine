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

	// ahh what the fuck is this?
	// sfml is bugged, and overwrites the next 4 bytes after the sf::Event, so we must pad it
	sf::Event& evt;
	char eventSpace[sizeof(sf::Event)+4];

	mm::ivec2 lastMousePos;
};