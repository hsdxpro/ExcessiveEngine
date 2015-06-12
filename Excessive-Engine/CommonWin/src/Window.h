// Windows O.S window
#pragma once

#include "../Common/src/IWindow.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

class Window : public IWindow
{
public:
	Window(const rWindow& d);

	bool PopEvent(rWindowEvent* evt_out) override;

	void Close() override;

	void Present() override;

	// Getters
	bool IsOpen() const override;

	u16 GetClientW() const override;
	u16 GetClientH() const override;

	float GetClientAspectRatio() const override;

	void SetText(const wchar_t* text) override;

protected:
	sf::Window w;
	mm::ivec3 lastMousePos;
};