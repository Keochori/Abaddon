#pragma once
#include "ImGui/ImGui.h"

class ImGuiManager
{
public:
	ImGuiManager(HWND& aHWND);
	~ImGuiManager() = default;

	void Init();
	void Update();
	void BeginFrame();
	void EndFrame();

private:
	void SceneTab();
	void GameTab();
	void HierarchyTab();
	void InspectorTab();
	void AssetsTab();
	void ConsoleTab();

	HWND& myHWND;
};

