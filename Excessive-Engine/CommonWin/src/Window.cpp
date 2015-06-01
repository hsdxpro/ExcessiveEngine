#include "Window.h"

#include <cassert>
#include <limits>
#include "SFML\Graphics\Text.hpp"
#include "SFML\Graphics\Font.hpp"
#include "GL\glew.h"

sf::Font* font;
sf::Text* text;
sf::String* str;

Window::Window(const rWindow& d)
:lastMousePos(0, 0)
{
	w.create(sf::VideoMode(d.clientW, d.clientH), d.capText.c_str(), (u32)d.style);
	w.setVerticalSyncEnabled(true);

	text = new sf::Text();
	str = new sf::String("BAZDMEG");
	font = new sf::Font();
	if (!font->loadFromFile(Sys::getWorkDir() + sf::String("arial.ttf")))
	{
		assert(0);
	}
}

bool Window::popEvent(rWindowEvent* evt_out)
{
	assert(evt_out);

	sf::Event evt;
	if (!w.pollEvent(evt))
		return false;

	evt_out->msg = (eWindowMsg)(evt.type);

	// Key press release
	if (evt.type == sf::Event::EventType::KeyPressed || evt.type == sf::Event::EventType::KeyReleased) 
	{
		evt_out->mouseBtn = eMouseBtn::INVALID;

		evt_out->key = (eKey)((int)evt.key.code);
		evt_out->deltaX = 0;
		evt_out->deltaY = 0;
		evt_out->x = 0;
		evt_out->y = 0;
	}
	else if (evt.type == sf::Event::EventType::MouseMoved) 
	{
		evt_out->key = eKey::INVALID;
		evt_out->mouseBtn = eMouseBtn::INVALID;

		// TODO: worst idea ever
		if (lastMousePos.x == std::numeric_limits<int>::min()) {
			evt_out->deltaX = 0;
			evt_out->deltaY = 0;
		}
		else {
			evt_out->deltaX = evt.mouseMove.x - lastMousePos.x;
			evt_out->deltaY = evt.mouseMove.y - lastMousePos.y;
		}
		evt_out->x = evt.mouseMove.x;
		evt_out->y = evt.mouseMove.y;

		lastMousePos.x = evt.mouseMove.x;
		lastMousePos.y = evt.mouseMove.y;
	}
	else if (evt.type == sf::Event::EventType::MouseEntered) 
	{
		// TODO: This sign was used, to ensure 0 dx, dy delta mouse move when gain focus
		// BAD IDEA
		lastMousePos.x = std::numeric_limits<int>::min();
		//lastMousePos.y = std::numeric_limits<int>.min();
	}
	else if (evt.type == sf::Event::EventType::MouseButtonPressed || evt.type == sf::Event::EventType::MouseButtonReleased)	
	{
		evt_out->key = eKey::INVALID;
		evt_out->deltaX = 0;
		evt_out->deltaY = 0;

		evt_out->x = evt.mouseButton.x;
		evt_out->y = evt.mouseButton.y;
		evt_out->mouseBtn = (eMouseBtn)((int)evt.mouseButton.button + 1); //  1 cuz our enum start with (out of sfml custom "INVALID" enum member)
	}
	else if (evt.type == sf::Event::EventType::MouseWheelMoved)
	{
		evt_out->deltaY = evt.mouseWheel.delta;
	}
	else if (evt.type == sf::Event::EventType::Closed) 
	{
		close();
	}
	else if (evt.type == sf::Event::EventType::Resized) 
	{
		evt_out->x = evt.size.width;
		evt_out->y = evt.size.height;
	}

	return true;
}

void Window::close() 
{
	w.close();
}

void Window::present() 
{
	//w.setActive(true);

	// TODO: REFACTOR, REPLACE...
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(0, 0);
	
	glBindVertexBuffers(0, 16, nullptr, nullptr, nullptr);
	
	for (uint8_t i = 0; i < 16; i++)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, i, 0);	
		glBindVertexBuffer(i, 0, 0, 0);
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glVertexPointer(0, GL_FLOAT, 0, 0);

	w.resetGLStates();

	text->setString(*str);
	text->setFont(*font);
	text->setCharacterSize(30);
	text->setStyle(sf::Text::Bold);
	text->setColor(sf::Color::White);
	text->setPosition(100, 100);
	
	w.draw(*text);

	w.display();
}

bool Window::isOpen() const
{
	return w.isOpen();
}

u16 Window::getClientW() const 
{
	return w.getSize().x;
}

u16 Window::getClientH() const 
{
	return w.getSize().y;
}

float Window::getClientAspectRatio() const 
{
	const sf::Vector2u size = w.getSize();
	return (float)size.x / size.y;
}

void Window::setText(const wchar_t* text)
{
	w.setTitle(text);
}