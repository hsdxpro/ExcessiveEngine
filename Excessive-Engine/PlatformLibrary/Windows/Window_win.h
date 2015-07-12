// Windows O.S window
#pragma once

#include "../IWindow.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

class Window : public IWindow
{
public:
	Window(const rWindow& d);

	bool PopEvent(rWindowEvent* evt_out) override;

	void Close() override;

	void Present() override;

	void HideCursor() override;
	void ShowCursor() override;

	void SetTitle(const wchar_t* text) override;

	// Getters
	bool IsOpen() const override;

	u16 GetClientW() const override;
	u16 GetClientH() const override;

	float GetClientAspectRatio() const override;

	mm::vec2 GetCenterPos() const override;

protected:
	sf::Window w;
	mm::ivec3 lastMousePos;
};