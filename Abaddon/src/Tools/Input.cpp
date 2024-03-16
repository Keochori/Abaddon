#include "pch.h"
#include "Input.h"
#include <windowsx.h>

Input::Input()
{

}

Input::~Input()
{

}

bool Input::IsKeyDown(const int aKeyCode) const
{
	return myCurrentState[aKeyCode] && myPreviousState[aKeyCode];
}

bool Input::IsKeyPressed(const int aKeyCode) const
{
	return myCurrentState[aKeyCode] && !myPreviousState[aKeyCode];
}

bool Input::IsKeyUp(const int aKeyCode) const
{
	return !myCurrentState[aKeyCode] && myPreviousState[aKeyCode];
}

bool Input::IsMouseButtonDown(const int aMouseButtonCode) const
{
	return myCurrentState[aMouseButtonCode] && myPreviousState[aMouseButtonCode];
}

bool Input::IsMouseButtonPressed(const int aMouseButtonCode) const
{
	return myCurrentState[aMouseButtonCode] && !myPreviousState[aMouseButtonCode];
}

bool Input::IsMouseButtonUp(const int aMouseButtonCode) const
{
	return !myCurrentState[aMouseButtonCode] && myPreviousState[aMouseButtonCode];
}

POINT Input::GetMousePosition() const
{
	return myCurrentMousePos;
}

POINT Input::GetMouseDelta() const
{
	POINT myDelta;
	myDelta.x = myCurrentMousePos.x - myPreviousMousePos.x;
	myDelta.y = myCurrentMousePos.y - myPreviousMousePos.y;
	return myDelta;
}

void Input::ResetMouseDelta()
{
	myCurrentMousePos = myTentativeMousePos;
}

const bool Input::GetMouseInsideWindow()
{
	return  myMouseInsideWindow;
}

void Input::MouseInsideWindowUpdate(bool aState)
{
	myMouseInsideWindow = aState;
}

bool Input::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Keyboard
		case WM_KEYDOWN:
			myTentativeState[wParam] = true;
			return true;
		case WM_KEYUP:
			myTentativeState[wParam] = false;
			return true;

		// Mouse
		case WM_LBUTTONDOWN:
			myTentativeState[0x01] = true;
			return true;
		case WM_LBUTTONUP:
			myTentativeState[0x01] = false;
			return true;
		case WM_RBUTTONDOWN:
			myTentativeState[0x02] = true;
			return true;
		case WM_RBUTTONUP:
			myTentativeState[0x02] = false;
			return true;
		case WM_MBUTTONDOWN:
			myTentativeState[0x04] = true;
			return true;
		case WM_MBUTTONUP:
			myTentativeState[0x04] = false;
			return true;
		case WM_MOUSEMOVE:
			myTentativeMousePos.x = LOWORD(lParam);
			myTentativeMousePos.y = HIWORD(lParam);
			return true;

	}
}

void Input::Update()
{
	myPreviousState = myCurrentState;
	myCurrentState = myTentativeState;
	myPreviousMousePos = myCurrentMousePos;
	myCurrentMousePos = myTentativeMousePos;
}