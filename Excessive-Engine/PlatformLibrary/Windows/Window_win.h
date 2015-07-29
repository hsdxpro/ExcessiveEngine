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

protected:
	sf::Window w;
	mm::ivec3 lastMousePos;
};