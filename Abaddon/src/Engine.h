#pragma once

class DX11;
class Renderer;
class Scene;

#ifdef enableImGui
class ImGuiManager;
#endif

enum eDebugMode
{
	Null = 1,
	Wireframe = 2,
	BoneWeights = 3,
};

class Engine
{
public:
	Engine(HWND& aHWND);
	~Engine();

	void Init();
	void Update();
	void BeginFrame();
	void EndFrame();


private:
	// Debug stuff
	void UpdateDebugModeInput();
	eDebugMode myDebugMode = eDebugMode::Null;
	int myBoneId = 1;

	HWND& myHWND;
	std::shared_ptr<DX11> myFramework;
	std::shared_ptr<Renderer> myRenderer;
	std::shared_ptr<Scene> myScene;

#ifdef enableImGui
	std::shared_ptr<ImGuiManager> myImGui;
#endif

	float myClearColor[4] = { 0.2f,0.2f,0.2f,1.0f };
};

