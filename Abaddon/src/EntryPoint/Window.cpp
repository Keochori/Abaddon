#include "pch.h"
#include "Window.h"

#include <windows.h>
#include "../Resources/resource.h"
#include <filesystem>
#include "Tools/Input.h"
#include "Tools/Timer.h"

#include "ImGui/ImGui.h"

#pragma warning(disable : 28251)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Window::Window() : myHInstance(GetModuleHandle(nullptr))
{
	// Set working directory
	std::filesystem::path currentPath = std::filesystem::current_path();

	if (IsDebuggerPresent())
	{
		currentPath.append("../Assets");
	}
	else
	{
		currentPath.append("../../Assets");
	}

	std::filesystem::current_path(currentPath);

	// Register the window class.
	LPCWSTR className = L"Abaddon";
	CreateAndRegisterWindowClass(myHInstance, className);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		className,						// Window class
		L"Abaddon",						// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,							// Parent window    
		NULL,							// Menu
		myHInstance,					// Instance handle
		NULL							// Additional application data
	);

	if (hwnd == NULL)
	{
		LOG_ERROR("Failed to create window");
		Assert(false);
	}

	myHWND = hwnd;

	// Set window position and size
	int width = 1500;
	int height = 902;

	SetWindowPos(hwnd, nullptr, 190, 80, width, height, 0);
	ShowWindow(hwnd, SW_SHOW);
}

Window::~Window()
{
}

void Window::CreateAndRegisterWindowClass(HINSTANCE& hInstance, LPCWSTR className)
{
	LPCWSTR CLASS_NAME = className;

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));

	RegisterClass(&wc);
}

bool Window::ProcessMessages()
{
	MSG msg = { };

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

HWND& Window::GetHWND()
{
	return myHWND;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef enableImGui
	// ImGui
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;
#endif
	
	Input::GetInstance().UpdateEvents(uMsg, wParam, lParam);
	Timer::GetInstance().Update();

	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));

			EndPaint(hwnd, &ps);
		}
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
