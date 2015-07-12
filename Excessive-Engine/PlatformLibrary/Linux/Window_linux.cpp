#include "Window.h"

Window::Window(const IWindow::rDesc& d) {
	init(d);
}

void Window::init(const IWindow::rDesc& d) {

}

bool Window::popEvent(IWindow::rEvent* evt_out) {
	// TODO
	return true;
}

void Window::close() {

}

void Window::displayClientRect() {

}

bool Window::isOpen() {
	// TODO
	return true;
}