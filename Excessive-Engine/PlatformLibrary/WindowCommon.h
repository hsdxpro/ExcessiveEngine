// Interface for typical O.S Window
#pragma once

#include "..\SupportLibrary\BasicTypes.h"
#include "Sys.h"
#include <string>

enum class eWindowStyle
{
	NO_BORDER_NO_TITLE = 0,						///< No border / title bar (this flag and all others are mutually exclusive)
	TITLE__FIXEDBORDER = 1 << 0,				///< Title bar + fixed border
	TITLE__RESIZABLE__MAXIMIZEBUTTON = 1 << 1,	///< Titlebar + resizable border + maximize button
	TITLE__CLOSE = 1 << 2,						///< Titlebar + close button
	FULLSCREEN = 1 << 3,						///< Fullscreen mode (this flag and all others are mutually exclusive)

	TITLE__RESIZE__CLOSE = TITLE__FIXEDBORDER | TITLE__RESIZABLE__MAXIMIZEBUTTON | TITLE__CLOSE ///< Default window style
};

enum class eWindowMsg 
{
	CLOSE,						///< The window requested to be closed (no data)
	RESIZE,						///< The window was resized (data in event.size)
	DEFOCUS,					///< The window lost the focus (no data)
	FOCUS,						///< The window gained the focus (no data)
	TEXT_ENTERED,				///< A character was entered (data in event.text)
	KEY_PRESS,					///< A key was pressed (data in event.key)
	KEY_RELEASE,				///< A key was released (data in event.key)
	MOUSE_WHEEL,				///< The mouse wheel was scrolled (data in event.mouseWheel)
	MOUSE_PRESS,				///< A mouse button was pressed (data in event.mouseButton)
	MOUSE_RELEASE,				///< A mouse button was released (data in event.mouseButton)
	MOUSE_MOVE,					///< The mouse cursor moved (data in event.mouseMove)
	MOUSE_ENTER,				///< The mouse cursor entered the area of the window (no data)
	MOUSE_LEAVE,				///< The mouse cursor left the area of the window (no data)
	JOYSTICK_BUTTON_PRESS,		///< A joystick button was pressed (data in event.joystickButton)
	JOYSTICK_BUTTON_RELEASE,	///< A joystick button was released (data in event.joystickButton)
	JOYSTICK_MOVE,				///< The joystick moved along an axis (data in event.joystickMove)
	JOYSTICK_CONNECT,			///< A joystick was connected (data in event.joystickConnect)
	JOYSTICK_DISCONNECT,		///< A joystick was disconnected (data in event.joystickConnect)
	COUNT						///< Keep last -- the total number of event types
};

struct rWindowEvent 
{
	rWindowEvent() : msg(eWindowMsg::COUNT), key(eKey::COUNT), deltaX(0), deltaY(0), x(0), y(0){}

	eWindowMsg msg;
	eKey key;
	eMouseBtn mouseBtn;
	i32 deltaX;
	i32 deltaY;
	i32 x;
	i32 y;
};

// Descriptor of window
struct rWindow
{
	rWindow() : clientW(0), clientH(0), style(eWindowStyle::TITLE__RESIZE__CLOSE){}

	u16 clientW;
	u16 clientH;
	eWindowStyle style;
	std::string capText;
};