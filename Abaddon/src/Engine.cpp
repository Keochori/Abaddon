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
	myRenderer->ChangeDebugMode(myDebugMode);

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
	UpdateDebugModeInput();

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

void Engine::UpdateDebugModeInput()
{
	bool clicked = false;

	if (Input::GetInstance().IsKeyPressed((int)eKeys::F1))
	{
		myDebugMode = eDebugMode::Null;
		clicked = true;
	}
	else if (Input::GetInstance().IsKeyPressed((int)eKeys::F2))
	{
		myDebugMode = eDebugMode::Wireframe;
		DX11::ToggleWireframeRS(true);
		clicked = true;
	}
	else if (Input::GetInstance().IsKeyPressed((int)eKeys::F3))
	{
		myDebugMode = eDebugMode::BoneWeights;
		clicked = true;
	}

	if (clicked)
	{
		if (myDebugMode != eDebugMode::Wireframe)
			DX11::ToggleWireframeRS(false);

		myBoneId = 0;
		myRenderer->ChangeBoneId(myBoneId);
		myRenderer->ChangeDebugMode((int)myDebugMode);
	}

	if (myDebugMode == eDebugMode::BoneWeights)
	{
		if (Input::GetInstance().IsKeyPressed((int)eKeys::SPACE))
		{
			if (myRenderer->ResetBoneId())
				myBoneId = -1;
			myBoneId++;
			myRenderer->ChangeBoneId(myBoneId);
		}
	}
}
