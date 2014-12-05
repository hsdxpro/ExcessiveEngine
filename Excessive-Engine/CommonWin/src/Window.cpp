#include "Window.h"
#include "SFML\Window\Window.hpp"



Window::Window(const IWindow::rDesc& d) {
	sf::Window w;
	w.create(sf::VideoMode(d.clientW, d.clientH), d.capText);
	w.setVerticalSyncEnabled(true);
}

bool Window::popEvent(IWindow::rEvent* evt_out) {
	// TODO
	return true;}

void Window::close() {

}

void Window::displayClientRect() {

}

bool Window::isOpen() {
	// TODO
	return true;
}