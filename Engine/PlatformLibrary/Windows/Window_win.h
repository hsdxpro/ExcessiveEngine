// Windows O.S window
#pragma once

#include "../WindowCommon.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

class Window
{
public:
	Window(const rWindow& d);

	bool PopEvent(rWindowEvent& evt_out);

	void Close();

	void Present();

	void SetTitle(const std::string& text);

	void SetCursorVisible(bool bVisible);

	// Getters
	bool IsOpen() const;

	u16 GetClientW() const;
	u16 GetClientH() const;

	float GetClientAspectRatio() const;

	mm::vec2 GetCenterPos() const;

private:
	eWindowMsg	ConvertSFMLWindowMsg(sf::Event::EventType windowMsg);
	eMouseBtn	ConvertSFMLMouseBtn(sf::Mouse::Button btn);
	eKey		ConvertSFMLKey(sf::Keyboard::Key key);

protected:
	sf::Window w;
	mm::ivec2 lastMousePos;
};