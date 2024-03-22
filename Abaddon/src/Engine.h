#pragma once

class DX11;
class Renderer;
class Scene;

class Engine
{
public:
	Engine(HWND& aHWND);
	~Engine();

	void Init();
	void Update();
	void UpdateImGui();
	void BeginFrame();
	void EndFrame();

private:
	HWND& myHWND;
	std::shared_ptr<DX11> myFramework;
	std::shared_ptr<Renderer> myRenderer;
	std::shared_ptr<Scene> myScene;

	float myClearColor[4] = { 0.3f,0.4f,0.6f,1.0f };
	bool created = false;
};

