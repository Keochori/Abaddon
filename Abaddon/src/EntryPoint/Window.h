#pragma once
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window();
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void CreateAndRegisterWindowClass(HINSTANCE& hInstance, LPCWSTR className);
	bool ProcessMessages();

	HWND& GetHWND();

private:
	HINSTANCE myHInstance;
	HWND myHWND;
};

