#include "pch.h"
#include "Engine.h"

#include "Graphics/DX11.h"
#include "Graphics/Renderer.h"
#include "Scene/Scene.h"
#include "Tools/Input.h"

#include "ImGui/ImGui.h"

Engine::Engine(HWND& aHWND) : myHWND(aHWND)
{}

Engine::~Engine()
{}

void Engine::Init()
{
	myFramework = std::make_shared<DX11>(myHWND);
	myFramework->Initialize(true);

	myRenderer = std::make_shared<Renderer>();
	myRenderer->Init();

	myScene = std::make_shared<Scene>(myRenderer);
	myScene->Init();

	// ImGui
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

void Engine::Update()
{
	BeginFrame();
	//-----------------------

	myScene->Update();

	//-----------------------
	EndFrame();
}

void Engine::UpdateImGui()
{
	ImGui::ShowDemoWindow();
}

void Engine::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	UpdateImGui();
	ImGui::Render();

	myFramework->BindRenderTarget();
	myFramework->BeginFrame(myClearColor);
	Input::GetInstance().Update();
}

void Engine::EndFrame()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	myFramework->EndFrame();
}
