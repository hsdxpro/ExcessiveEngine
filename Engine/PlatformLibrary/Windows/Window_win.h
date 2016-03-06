// Window implementation on [Windows OS]
#pragma once

#include "../WindowCommon.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//#include "SFML/Graphics/RenderWindow.hpp"
//#include "SFML/Window/Event.hpp"
//#include "SupportLibrary/Common.h"

class Window
{
public:
	Window(const rWindow& d);
	~Window();

	bool PopEvent(rWindowEvent& evt_out);

	void Close();

	void Clear(const Color& color);

	void SetPos(const mm::ivec2& pos = mm::ivec2(0, 0));
	void SetSize(const mm::uvec2& size);

	void SetClientPixels(const Color* const pixels);

	void SetTitle(const std::string& text);

	void SetCursorVisible(bool bVisible);

	// Getters
	bool IsOpen() const;

	size_t GetHandle() const;

	u16 GetClientWidth() const;
	u16 GetClientHeight() const;

	unsigned GetNumClientPixels() const;
	float GetClientAspectRatio() const;

	mm::vec2 GetCenterPos() const;

private:
	//typedef UINT_PTR WPARAM;

	//eWindowMsg	ConvertFromSFMLWindowMsg(sf::Event::EventType windowMsg);
	//eMouseBtn	ConvertFromSFMLMouseBtn(sf::Mouse::Button btn);
	//eKey		ConvertFromSFMLKey(sf::Keyboard::Key key);
	eKey		ConvertFromWindowsKey(WPARAM key);
	//sf::Uint32	ConvertToSFMLWindowStyle(eWindowStyle style);

protected:
	HWND hwnd;
	bool bClosed;

	bool bGenerateSysKeyAltDown;
	bool bGenerateSysKeyAltUp;
	//sf::RenderWindow w;
	//mm::ivec2 lastMousePos;
};