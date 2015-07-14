#include "Window_win.h"

#include <cassert>
#include <limits>

Window::Window(const rWindow& d)
{
	lastMousePos.x = std::numeric_limits<int>::min();
	lastMousePos.y = std::numeric_limits<int>::min();

	w.create(sf::VideoMode(d.clientW, d.clientH), d.capText.c_str(), (u32)d.style);
	w.setVerticalSyncEnabled(true);
}

bool Window::PopEvent(rWindowEvent& evt_out)
{
	sf::Event evt;
	if (!w.pollEvent(evt))
		return false;

	evt_out.msg = (eWindowMsg)(evt.type);

	// Key press release
	if (evt.type == sf::Event::EventType::KeyPressed || evt.type == sf::Event::EventType::KeyReleased) 
	{
		evt_out.mouseBtn = eMouseBtn::INVALID;

		evt_out.key = (eKey)((int)evt.key.code);
		evt_out.deltaX = 0;
		evt_out.deltaY = 0;
		evt_out.x = 0;
		evt_out.y = 0;
	}
	else if (evt.type == sf::Event::EventType::MouseMoved) 
	{
		evt_out.key = eKey::INVALID;
		evt_out.mouseBtn = eMouseBtn::INVALID;

		// TODO: worst idea ever
		if (lastMousePos.x == std::numeric_limits<int>::min()) 
		{
			lastMousePos.x = evt.mouseMove.x;
			lastMousePos.y = evt.mouseMove.y;
		}

		evt_out.deltaX = evt.mouseMove.x - lastMousePos.x;
		evt_out.deltaY = evt.mouseMove.y - lastMousePos.y;
		
		evt_out.x = evt.mouseMove.x;
		evt_out.y = evt.mouseMove.y;

		lastMousePos.x = evt.mouseMove.x;
		lastMousePos.y = evt.mouseMove.y;
	}
	else if (evt.type == sf::Event::EventType::MouseEntered) 
	{
		// TODO: This sign was used, to ensure 0 dx, dy delta mouse move when gain focus
		// BAD IDEA
		//lastMousePos.x = std::numeric_limits<int>::min();
		//lastMousePos.y = std::numeric_limits<int>.min();
	}
	else if (evt.type == sf::Event::EventType::MouseButtonPressed || evt.type == sf::Event::EventType::MouseButtonReleased)	
	{
		evt_out.key = eKey::INVALID;
		evt_out.deltaX = 0;
		evt_out.deltaY = 0;

		evt_out.x = evt.mouseButton.x;
		evt_out.y = evt.mouseButton.y;
		evt_out.mouseBtn = (eMouseBtn)((int)evt.mouseButton.button + 1); //  1 cuz our enum start with (out of sfml custom "INVALID" enum member)
	}
	else if (evt.type == sf::Event::EventType::MouseWheelMoved)
	{
		evt_out.deltaY = evt.mouseWheel.delta;
	}
	else if (evt.type == sf::Event::EventType::Closed) 
	{
		Close();
	}
	else if (evt.type == sf::Event::EventType::Resized) 
	{
		evt_out.x = evt.size.width;
		evt_out.y = evt.size.height;
	}

	return true;
}

void Window::Close() 
{
	w.close();
}

void Window::Present() 
{
	w.display();
}

void Window::HideCursor()
{
	w.setMouseCursorVisible(false);
}

void Window::ShowCursor()
{
	w.setMouseCursorVisible(true);
}

void Window::SetTitle(const wchar_t* text)
{
	w.setTitle(text);
}

bool Window::IsOpen() const
{
	return w.isOpen();
}

u16 Window::GetClientW() const 
{
	return w.getSize().x;
}

u16 Window::GetClientH() const 
{
	return w.getSize().y;
}

float Window::GetClientAspectRatio() const 
{
	const sf::Vector2u size = w.getSize();
	return (float)size.x / size.y;
}

mm::vec2 Window::GetCenterPos() const
{
	auto size = w.getSize();
	auto pos = w.getPosition();

	return mm::vec2(pos.x + size.x * 0.5, pos.y + size.y * 0.5);
}