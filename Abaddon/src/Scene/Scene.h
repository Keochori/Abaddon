#pragma once
#include "Camera.h"
#include "EnTT/entt.hpp"
#include "Graphics/Renderer.h"

class Entity;

class Scene
{
public:
	Scene(Renderer& aRenderer);
	~Scene();

	void Init();
	void Update();

	Entity CreateEntity(std::string aName = "Unnamed");

	std::shared_ptr<Camera> GetCamera();

private:
	friend class Entity;

	Renderer& myRenderer;
	std::shared_ptr<Camera> myCamera;
	entt::registry myRegistry;
};

