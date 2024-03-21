#include "pch.h"
#include <windows.h>
#include <iostream>
#include "../Resources/resource.h"
#include <filesystem>
#include "Graphics/DX11.h"
#include "Scene/Scene.h"
#include "Graphics/Renderer.h"

#include "Tools/Input.h"

#include "imgui.h"
#include "ImGui/ImGuiBuilder.h"

#pragma warning(disable : 28251)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

FILE* CreateConsole()
{
	FILE* consoleStream;
	AllocConsole();
	freopen_s(&consoleStream, "CONIN$", "r", stdin);
	freopen_s(&consoleStream, "CONOUT$", "w", stdout);
	freopen_s(&consoleStream, "CONOUT$", "w", stderr);

	LOG_SUCCESS("Console successfully initiated.");

	return consoleStream;
}

void CreateAndRegisterWindowClass(HINSTANCE& hInstance, LPCWSTR className)
{
	LPCWSTR CLASS_NAME = className;

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));

	RegisterClass(&wc);
}

static float rotation = 0;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
#pragma region SetWorkDir
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
#pragma endregion

	// Create console
	FILE* consoleStream = CreateConsole();

	// Register the window class.
	LPCWSTR className = L"Abaddon";
	CreateAndRegisterWindowClass(hInstance, className);

	// Create the window.
#pragma region CreateWindow
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		className,                     // Window class
		L"Abaddon",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		LOG_ERROR("Failed to create window");
		Assert(false);
		return 0;
	}
#pragma endregion

	// Set window position and size
	int width = 1366;
	int height = 768;

	SetWindowPos(hwnd, nullptr, 200, 200, width, height, 0);
	ShowWindow(hwnd, nCmdShow);
	// ---------------------------------------------------------------

	// Create framework
	DX11 framework(hwnd);
	framework.Initialize(true);

	float clearColor[4] = { 0.3f,0.4f,0.6f,1.0f };

	// Create renderer
	Renderer renderer;
	renderer.Init();

	// Create scene
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(renderer);
	scene->Init();

	// ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(DX11::ourDevice.Get(), DX11::ourContext.Get());

	// Loop
	bool running = true;
	MSG msg = { };
	while (running)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				// End program
				consoleStream = nullptr;
				delete consoleStream;

				ImGui_ImplDX11_Shutdown();
				ImGui_ImplWin32_Shutdown();
				ImGui::DestroyContext();

				running = false;
				return 0;
			}
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();

		ImGui::Render();
		// Re-bind Target after ImGui changes it (I think)
		framework.BindRenderTarget();

		framework.BeginFrame(clearColor);
		Input::GetInstance().Update();

		// Game loop --------------
		scene->Update();
		// ------------------------

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		framework.EndFrame();
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ImGui
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	Input::GetInstance().UpdateEvents(uMsg, wParam, lParam);

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

		case WM_DPICHANGED:
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
			{
				//const int dpi = HIWORD(wParam);
				//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
				const RECT* suggested_rect = (RECT*)lParam;
				::SetWindowPos(hwnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}