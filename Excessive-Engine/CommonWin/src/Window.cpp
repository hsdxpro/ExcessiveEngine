#include "Window.h"
#include "SFML\Window\Event.hpp"

#include <cassert>

Window::Window(const rWindow& d) {
	w.create(sf::VideoMode(d.clientW, d.clientH), d.capText.c_str());
	w.setVerticalSyncEnabled(true);
}

bool Window::popEvent(rWindowEvent* evt_out) {
	bool b = w.pollEvent(evt);
	
	// Ez most baromira veletlen egybe eses hogy SFML - bol hasznalja a Sys a key - eket
	if (evt.type == sf::Event::EventType::KeyPressed || evt.type == sf::Event::EventType::KeyReleased)
		evt_out->key = (eKey)((int)evt.key.code);
	
	evt_out->msg = (eWindowMsg)((u32)evt.type);

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