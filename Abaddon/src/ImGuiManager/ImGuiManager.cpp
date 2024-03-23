#include "pch.h"
#include "ImGuiManager.h"
#include "Graphics/DX11.h"

ImGuiManager::ImGuiManager(HWND& aHWND) : myHWND(aHWND)
{}

void ImGuiManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	ImGui_ImplWin32_Init(myHWND);
	ImGui_ImplDX11_Init(DX11::ourDevice.Get(), DX11::ourContext.Get());
}

void ImGuiManager::Update()
{
	//ImGui::ShowDemoWindow();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	SceneTab();
	GameTab();
	HierarchyTab();
	InspectorTab();
	AssetsTab();
	ConsoleTab();
}

void ImGuiManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	Update();
	ImGui::Render();
}

void ImGuiManager::EndFrame()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ImGuiManager::SceneTab()
{
	ImGui::Begin("Scene");
	ImGui::Image((void*)DX11::ourTextureSRV.Get(), ImGui::GetWindowSize());
	ImGui::End();
}

void ImGuiManager::GameTab()
{
	ImGui::Begin("Game");
	ImGui::End();
}

void ImGuiManager::HierarchyTab()
{
	ImGui::Begin("Hierarchy");
	ImGui::End();
}

void ImGuiManager::InspectorTab()
{
	ImGui::Begin("Inspector");
	ImGui::End();
}

void ImGuiManager::AssetsTab()
{
	ImGui::Begin("Assets");
	ImGui::End();
}

void ImGuiManager::ConsoleTab()
{
	ImGui::Begin("Console");
	ImGui::End();
}
