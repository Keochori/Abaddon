#pragma once

class DX11;
class Renderer;
class Scene;

#ifdef enableImGui
class ImGuiManager;
#endif

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
	HWND& myHWND;
	std::shared_ptr<DX11> myFramework;
	std::shared_ptr<Renderer> myRenderer;
	std::shared_ptr<Scene> myScene;

#ifdef enableImGui
	std::shared_ptr<ImGuiManager> myImGui;
#endif

	float myClearColor[4] = { 0.2f,0.2f,0.2f,1.0f };
};

