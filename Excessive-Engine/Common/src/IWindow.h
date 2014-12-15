// Interface for typical O.S Window
#pragma once

#include "BasicTypes.h"
#include "Sys.h"

// TMP, REMOVE THAT
#include <string>

// Descriptor of window
struct rWindow {
	rWindow() : clientW(0), clientH(0){}

	u16 clientW;
	u16 clientH;
	std::string capText;
};

enum class eWindowMsg {
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

struct rWindowEvent {
	rWindowEvent() : msg(eWindowMsg::COUNT), key(eKey::COUNT), mouseDeltaX(0), mouseDeltaY(0), mouseX(0), mouseY(0) {}

	eWindowMsg msg;
	eKey key;
	eMouseBtn mouseBtn;
	i32 mouseDeltaX;
	i32 mouseDeltaY;
	i32 mouseX;
	i32 mouseY;
};

class IWindow
{
public:
	// Simply closes the window so you will not need it
	virtual void close() = 0;

	// This will give you the next event in the queue that Operating System transmitted to the window, and remove it from the queue
	virtual bool popEvent(rWindowEvent* evt_out) = 0;

	// U need to call that function after some DrawingAPI finishe s draw on window client region, this will BLIT the content to your monitor where client region is defined
	virtual void displayClientRect() = 0;

	// Window client surface width
	virtual u16 getClientW() const = 0;

	// Window client surface height
	virtual u16 getClientH() const = 0;

	// (client surface width / client surface height)
	virtual float getClientAspectRatio() const = 0;

	// Returns true if the window is currently opened, else (false) closed
	virtual bool isOpen() const = 0;

	/// Set window title
	virtual void setText(const wchar_t* text) = 0;
};