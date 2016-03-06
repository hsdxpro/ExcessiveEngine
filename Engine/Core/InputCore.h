#pragma once
#include "..\PlatformLibrary\Sys.h"
#include <array>
#include <functional>
#include <vector>

class InputCore
{
public:
	InputCore();

public:
	void KeyPress(eKey key);
	void KeyRelease(eKey key);

	void MouseRightPress();
	void MouseRightRelease();
	void MouseLeftPress();
	void MouseLeftRelease();
	void MouseMidPress();
	void MouseMidRelease();
	
	void MouseMove(const mm::ivec2& mouseDelta, const mm::ivec2& clientMousePos);

	void RegOnKeyDown(eKey key, const std::function<void()> callb);
	void RegOnKeyPressed(eKey key, const std::function<void()> callb);
	void RegOnKeyReleased(eKey key, const std::function<void()> callb);

	void RegOnMouseRightPressed(  const std::function<void(const mm::uvec2& clientMousePos)> callb);
	void RegOnMouseRightReleased( const std::function<void(const mm::uvec2& clientMousePos)> callb);
	void RegOnMouseRightDown(	  const std::function<void(const mm::uvec2& clientMousePos)> callb);
	void RegOnMouseLeftPressed(	  const std::function<void(const mm::uvec2& clientMousePos)> callb);
	void RegOnMouseLeftReleased(  const std::function<void(const mm::uvec2& clientMousePos)> callb);
	void RegOnMouseLeftDown(	  const std::function<void(const mm::uvec2& clientMousePos)> callb);
	void RegOnMouseMidPressed(	  const std::function<void(const mm::uvec2& clientMousePos)> callb);
	void RegOnMouseMidReleased(	  const std::function<void(const mm::uvec2& clientMousePos)> callb);
	void RegOnMouseMidDown(		  const std::function<void(const mm::uvec2& clientMousePos)> callb);
	void RegOnMouseMove(		  const std::function<void(const mm::ivec2& mouseDelta, const mm::uvec2& clientMousePos)> callb);

	void ClearFrameData();
	void Update();

	bool IsKeyDown(eKey key);
	bool IsKeyPressed(eKey key);
	bool IsKeyReleased(eKey key);

	bool IsRightMouseBtnPressed();
	bool IsRightMouseBtnReleased();
	bool IsRightMouseBtnDown();
	bool IsLeftMouseBtnPressed();
	bool IsLeftMouseBtnReleased();
	bool IsLeftMouseBtnDown();
	bool IsMidMouseBtnPressed();
	bool IsMidMouseBtnReleased();
	bool IsMidMouseBtnDown();
	
	bool IsMouseMove(mm::ivec2& mouseDelta_out);

protected:
	struct rKeyDownInfo
	{
		bool bDownCurFrame;
		bool bDownPrevFrame;
	};

	// Storing key downs (current, previous) frame
	std::array<rKeyDownInfo, (size_t)COUNT_eKey> keyDownArray;

	// Mouse inputs
	bool bMouseRightDownCurFrame;
	bool bMouseRightDownPrevFrame;
	bool bMouseLeftDownCurFrame;
	bool bMouseLeftDownPrevFrame;
	bool bMouseMidDownCurFrame;
	bool bMouseMidDownPrevFrame;
	mm::ivec2 mouseDelta;
	mm::ivec2 clientMousePos;

	// Registered keyboard callbacks
	std::vector<std::pair<eKey, std::function<void()>>> onKeyDownCallbacks;
	std::vector<std::pair<eKey, std::function<void()>>> onKeyPressedCallbacks;
	std::vector<std::pair<eKey, std::function<void()>>> onKeyReleasedCallbacks;

	// Registered mouse callbacks
	std::vector<std::function<void(const mm::uvec2& clientMousePos)>> onMouseRightPressedCallbacks;
	std::vector<std::function<void(const mm::uvec2& clientMousePos)>> onMouseRightReleasedCallbacks;
	std::vector<std::function<void(const mm::uvec2& clientMousePos)>> onMouseRightDownCallbacks;
	std::vector<std::function<void(const mm::uvec2& clientMousePos)>> onMouseLeftPressedCallbacks;
	std::vector<std::function<void(const mm::uvec2& clientMousePos)>> onMouseLeftReleasedCallbacks;
	std::vector<std::function<void(const mm::uvec2& clientMousePos)>> onMouseLeftDownCallbacks;
	std::vector<std::function<void(const mm::uvec2& clientMousePos)>> onMouseMidPressedCallbacks;
	std::vector<std::function<void(const mm::uvec2& clientMousePos)>> onMouseMidReleasedCallbacks;
	std::vector<std::function<void(const mm::uvec2& clientMousePos)>> onMouseMidDownCallbacks;
	std::vector<std::function<void(const mm::ivec2& mouseDelta, const mm::uvec2& clientMousePos)>> onMouseMoveCallbacks;
};

extern InputCore Input;