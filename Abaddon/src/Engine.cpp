#include "pch.h"
#include "Engine.h"

#include "Graphics/DX11.h"
#include "Graphics/Renderer.h"
#include "Scene/Scene.h"
#include "Tools/Input.h"

#ifdef enableImGui
#include "ImGuiManager/ImGuiManager.h"
#endif


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

#ifdef enableImGui
	// ImGui
	myImGui = std::make_shared<ImGuiManager>(myHWND);
	myImGui->Init();
#endif
}

void Engine::Update()
{
	BeginFrame();
	//-----------------------

	myScene->Update();

	//-----------------------
	EndFrame();
}

void Engine::BeginFrame()
{
#ifdef enableImGui
		myImGui->BeginFrame();
#endif

	myFramework->BindRenderTarget();
	myFramework->BeginFrame(myClearColor);
	Input::GetInstance().Update();

#ifdef enableImGui
		DX11::BindRenderTargetTexture();
#endif
}

void Engine::EndFrame()
{
#ifdef enableImGui
		DX11::BindRenderTarget();
		myImGui->EndFrame();
#endif

	myFramework->EndFrame();
}
