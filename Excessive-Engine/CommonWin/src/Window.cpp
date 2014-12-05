#include "Window.h"
#include "SFML\Window\Event.hpp"

#include <cassert>

Window::Window(const IWindow::rDesc& d) {
	w.create(sf::VideoMode(d.clientW, d.clientH), d.capText.c_str());
	w.setVerticalSyncEnabled(true);
}

bool Window::popEvent(IWindow::rEvent* evt_out) {
	bool b = w.pollEvent(evt);
	
	// Ez most baromira veletlen egybe eses hogy SFML - bol hasznalja a Sys a key - eket
	if (evt.type == sf::Event::EventType::KeyPressed || evt.type == sf::Event::EventType::KeyReleased)
		evt_out->key = (Sys::eKey)((int)evt.key.code);
	
	evt_out->msg = (eMsg)((unsigned)evt.type);

	if (evt_out->msg == IWindow::eMsg::CLOSE)
		close();

	return b;
}

void Window::close() {
	w.close();
}

void Window::displayClientRect() {
	w.display();
}

bool Window::isOpen() {
	// TODO
	return true;
}