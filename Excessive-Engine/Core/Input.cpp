#include "Input.h"

Input gInput;

Input::Input()
{
	for(auto& keyDownInfo : keyDownArray)
	{
		keyDownInfo.bDownCurFrame = false;
		keyDownInfo.bDownPrevFrame = false;
	}

	bMouseRightDownCurFrame = false;
	bMouseRightDownPrevFrame = false;
	bMouseLeftDownCurFrame = false;
	bMouseLeftDownPrevFrame = false;
	bMouseMidDownCurFrame = false;
	bMouseMidDownPrevFrame = false;

	mouseDelta.x = 0;
	mouseDelta.y = 0;
	clientMousePos.x = 0;
	clientMousePos.y = 0;
}

void Input::KeyPress(eKey key)
{
	keyDownArray[(size_t)key].bDownCurFrame = true;
	keyDownArray[(size_t)key].bDownPrevFrame = false;

	// Dispatch registered callbacks binded to that key
	for (auto& info : onKeyPressedCallbacks)
		if (info.first == key)
			info.second();
}

void Input::KeyRelease(eKey key)
{
	keyDownArray[(size_t)key].bDownCurFrame = false;
	keyDownArray[(size_t)key].bDownPrevFrame = true;

	// Dispatch registered callbacks binded to that key
	for (auto& info : onKeyReleasedCallbacks)
		if (info.first == key)
			info.second();
}

void Input::MouseRightPress()
{
	bMouseRightDownCurFrame = true;
	bMouseRightDownPrevFrame = false;

	// Dispatch callbacks binded to right mouse press
	for (auto& callb : onMouseRightPressedCallbacks)
		callb(clientMousePos);
}

void Input::MouseRightRelease()
{
	bMouseRightDownCurFrame = false;
	bMouseRightDownPrevFrame = true;

	// Dispatch callbacks binded to right mouse release
	for (auto& callb : onMouseRightReleasedCallbacks)
		callb(clientMousePos);
}

void Input::MouseLeftPress()
{
	bMouseLeftDownCurFrame = true;
	bMouseLeftDownPrevFrame = false;

	// Dispatch callbacks binded to left mouse press
	for(auto& callb : onMouseLeftPressedCallbacks)
		callb(clientMousePos);
}

void Input::MouseLeftRelease()
{
	bMouseLeftDownCurFrame = false;
	bMouseLeftDownPrevFrame = true;

	// Dispatch callbacks binded to left mouse release
	for (auto& callb : onMouseLeftReleasedCallbacks)
		callb(clientMousePos);
}

void Input::MouseMidPress()
{
	bMouseMidDownCurFrame = true;
	bMouseMidDownPrevFrame = false;

	// Dispatch callbacks binded to mid mouse press
	for (auto& callb : onMouseMidPressedCallbacks)
		callb(clientMousePos);
}

void Input::MouseMidRelease()
{
	bMouseMidDownCurFrame = false;
	bMouseMidDownPrevFrame = true;

	// Dispatch callbacks binded to mid mouse release
	for (auto& callb : onMouseMidReleasedCallbacks)
		callb(clientMousePos);
}

void Input::MouseMove(const mm::ivec2& mouseDelta, const mm::uvec2& clientMousePos)
{
	this->mouseDelta += mouseDelta;
	this->clientMousePos = clientMousePos;

	// Dispatch registered mouseMove callbacks
	for(auto& callb : onMouseMoveCallbacks)
		callb(mouseDelta, clientMousePos);
}

void Input::RegOnKeyDown(eKey key, const std::function<void()> callb)
{
	onKeyDownCallbacks.push_back(std::make_pair(key,callb));
}

void Input::RegOnKeyPressed(eKey key, const std::function<void()> callb)
{
	onKeyPressedCallbacks.push_back(std::make_pair(key,callb));
}

void Input::RegOnKeyReleased(eKey key, const std::function<void()> callb)
{
	onKeyReleasedCallbacks.push_back(std::make_pair(key,callb));
}

void Input::RegOnMouseRightPressed(const std::function<void(const mm::uvec2& mousePos)> callb)
{
	onMouseRightPressedCallbacks.push_back(callb);
}

void Input::RegOnMouseRightReleased(const std::function<void(const mm::uvec2& mousePos)>  callb)
{
	onMouseRightReleasedCallbacks.push_back(callb);
}

void Input::RegOnMouseRightDown(const std::function<void(const mm::uvec2& mousePos)>  callb)
{
	onMouseRightDownCallbacks.push_back(callb);
}

void Input::RegOnMouseLeftPressed(const std::function<void(const mm::uvec2& mousePos)>  callb)
{
	onMouseLeftPressedCallbacks.push_back(callb);
}

void Input::RegOnMouseLeftReleased(const std::function<void(const mm::uvec2& mousePos)>  callb)
{
	onMouseLeftReleasedCallbacks.push_back(callb);
}

void Input::RegOnMouseLeftDown(const std::function<void(const mm::uvec2& mousePos)>  callb)
{
	onMouseLeftDownCallbacks.push_back(callb);
}

void Input::RegOnMouseMidPressed(const std::function<void(const mm::uvec2& mousePos)>  callb)
{
	onMouseMidPressedCallbacks.push_back(callb);
}

void Input::RegOnMouseMidReleased(const std::function<void(const mm::uvec2& mousePos)>  callb)
{
	onMouseMidReleasedCallbacks.push_back(callb);
}

void Input::RegOnMouseMidDown(const std::function<void(const mm::uvec2& mousePos)>  callb)
{
	onMouseMidDownCallbacks.push_back(callb);
}

void Input::RegOnMouseMove(const std::function<void(const mm::ivec2& mouseDelta, const mm::uvec2& clientMousePos)> callb)
{
	onMouseMoveCallbacks.push_back(callb);
}

void Input::ClearFrameData()
{

	// General equation downPrevFrame = downCurFrame | (downPrevFrame & downCurFrame)
	//// Simply if button is down, then prevFrame must become true cuz 1 frame passed...
	// and if button is not down, but prevFrame true, this means we release button previous frame, so 1 frame passed, and prevFrame must become false
	// eq. pseudo
	////if(bMouseLeftDownPrevFrame && !bMouseLeftDownCurFrame)
	////	bMouseLeftDownPrevFrame = false;
	////0 0 -> false
	////0 1 -> false
	////1 0 -> false
	////1 1 -> true

	// Updating key downs with general equation
	for (auto& keyDownInfo : keyDownArray)
		keyDownInfo.bDownPrevFrame = keyDownInfo.bDownCurFrame | (keyDownInfo.bDownPrevFrame & keyDownInfo.bDownCurFrame);
		
	// Updating mouse downs with general equation
	bMouseRightDownPrevFrame = bMouseRightDownCurFrame | (bMouseRightDownPrevFrame & bMouseRightDownCurFrame);
	bMouseLeftDownPrevFrame = bMouseLeftDownCurFrame | (bMouseLeftDownPrevFrame & bMouseLeftDownCurFrame);
	bMouseMidDownPrevFrame = bMouseMidDownCurFrame | (bMouseMidDownPrevFrame & bMouseMidDownCurFrame);

	mouseDelta.x = 0;
	mouseDelta.y = 0;
}

void Input::Update()
{
	for(size_t i = 0; i < keyDownArray.size(); i++)
	{
		// Leave keys not down
		if(!keyDownArray[i].bDownCurFrame)
			continue;

		// Search if that key have callbacks binded, dispatch them
		for(auto& info : onKeyDownCallbacks)
			if(info.first == (eKey)i)
				info.second();
	}

	if(bMouseLeftDownCurFrame)
		for (auto& callb : onMouseLeftDownCallbacks)
			callb(clientMousePos);

	if (bMouseRightDownCurFrame)
		for (auto& callb : onMouseRightDownCallbacks)
			callb(clientMousePos);

	if (bMouseMidDownCurFrame)
		for (auto& callb : onMouseMidDownCallbacks)
			callb(clientMousePos);
}

bool Input::IsKeyDown(eKey key)
{
	return keyDownArray[(size_t)key].bDownCurFrame;
}

bool Input::IsKeyPressed(eKey key)
{
	return keyDownArray[(size_t)key].bDownCurFrame && !keyDownArray[(size_t)key].bDownPrevFrame;
}

bool Input::IsKeyReleased(eKey key)
{
	return keyDownArray[(size_t)key].bDownPrevFrame && !keyDownArray[(size_t)key].bDownCurFrame;
}

bool Input::IsMouseRightPressed()
{
	return bMouseRightDownCurFrame && !bMouseRightDownPrevFrame;
}

bool Input::IsMouseRightReleased()
{
	return bMouseRightDownPrevFrame && !bMouseRightDownCurFrame;
}

bool Input::IsMouseRightDown()
{
	return bMouseRightDownCurFrame;
}

bool Input::IsMouseLeftPressed()
{
	return bMouseLeftDownCurFrame && !bMouseLeftDownPrevFrame;
}

bool Input::IsMouseLeftReleased()
{
	return bMouseLeftDownPrevFrame && !bMouseLeftDownCurFrame;
}

bool Input::IsMouseLeftDown()
{
	return bMouseLeftDownCurFrame;
}

bool Input::IsMouseMidPressed()
{
	return bMouseMidDownCurFrame && !bMouseMidDownPrevFrame;
}

bool Input::IsMouseMidReleased()
{
	return bMouseMidDownPrevFrame && !bMouseMidDownCurFrame;
}

bool Input::IsMouseMidDown()
{
	return bMouseMidDownCurFrame;
}

bool Input::IsMouseMove(mm::uvec2& mouseDelta_out)
{
	mouseDelta_out = mouseDelta;
	return mouseDelta.x != 0 || mouseDelta.y != 0;
}