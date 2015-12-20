// Window implementation on [Windows OS]
#pragma once

#include "../WindowCommon.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SupportLibrary/Common.h"

class Window
{
public:
	Window(const rWindow& d);
	~Window();

	bool PopEvent(rWindowEvent& evt_out);

	void Close();

	void Clear(const Color& color);
	void Present();

	void SetPos(const mm::ivec2& pos = mm::ivec2(0, 0));
	void SetSize(const mm::uvec2& pos);

	void SetClientPixels(const Color* const pixels);

	void SetTitle(const std::string& text);

	void SetCursorVisible(bool bVisible);

	// Getters
	bool IsOpen() const;

	u16 GetClientW() const;
	u16 GetClientH() const;

	unsigned GetNumClientPixels() const;
	float GetClientAspectRatio() const;

	mm::vec2 GetCenterPos() const;

private:
	eWindowMsg	ConvertFromSFMLWindowMsg(sf::Event::EventType windowMsg);
	eMouseBtn	ConvertFromSFMLMouseBtn(sf::Mouse::Button btn);
	eKey		ConvertFromSFMLKey(sf::Keyboard::Key key);
	sf::Uint32	ConvertToSFMLWindowStyle(eWindowStyle style);

protected:
	sf::RenderWindow w;
	mm::ivec2 lastMousePos;
};