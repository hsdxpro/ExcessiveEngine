#include "Window.h"
#include "SFML\Window\Event.hpp"

#include <cassert>

Window::Window(const rWindow& d) {

	w.create(sf::VideoMode(d.clientW, d.clientH), d.capText.c_str());
	w.setVerticalSyncEnabled(true);
}

bool Window::popEvent(rWindowEvent* evt_out) {
	bool b = w.pollEvent(evt);
	
	evt_out->msg = (eWindowMsg)((u32)evt.type);

	// Key press release
	if (evt.type == sf::Event::EventType::KeyPressed || evt.type == sf::Event::EventType::KeyReleased) {
		evt_out->mouseBtn = eMouseBtn::INVALID;

		evt_out->key = (eKey)((int)evt.key.code);
		evt_out->mouseDx = 0;
		evt_out->mouseDy = 0;
	} else 
	if (evt.type == sf::Event::EventType::MouseMoved) {
		evt_out->key == eKey::INVALID;
		evt_out->mouseBtn = eMouseBtn::INVALID;

		evt_out->mouseDx = evt.mouseMove.x;
		evt_out->mouseDy = evt.mouseMove.y;
		
	} else
	if (evt.type == sf::Event::EventType::MouseButtonPressed || evt.type == sf::Event::EventType::MouseButtonReleased)
	{
		evt_out->key == eKey::INVALID;
		evt_out->mouseDx = 0;
		evt_out->mouseDy = 0;

		evt_out->mouseDx = evt.mouseButton.x;
		evt_out->mouseDy = evt.mouseButton.y;
		evt_out->mouseBtn = (eMouseBtn)((int)evt.mouseButton.button + 1); //  1 cuz our enum start with (out of sfml custom "INVALID" enum member)
	} else
	if (evt_out->msg == eWindowMsg::CLOSE)
		close();

	return b;
}

void Window::close() {
	w.close();
}

void Window::displayClientRect() {
	w.display();
}

bool Window::isOpen() const {
	return w.isOpen();
}

u16 Window::getClientW() const {
	return w.getSize().x;
}

u16 Window::getClientH() const {
	return w.getSize().y;
}

float Window::getClientAspectRatio() const {
	const sf::Vector2u size = w.getSize();
	return (float)size.x / size.y;
}