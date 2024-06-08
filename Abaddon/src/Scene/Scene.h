#pragma once
#include "Camera.h"
#include "EnTT/entt.hpp"
#include "Graphics/Renderer.h"
#include "Graphics/Animation/Animator.h"

class Renderer;
class Entity;
struct ScriptComponent;

class Scene
{
public:
	Scene(std::shared_ptr<Renderer> aRenderer);
	~Scene();

	void Init();
	void Update();

	Entity CreateEntity(std::string aName = "Unnamed");

	std::shared_ptr<Camera> GetCamera();

private:
	friend class Entity;

	std::shared_ptr<Renderer> myRenderer;
	std::shared_ptr<Camera> myCamera;
	entt::registry myRegistry;
	Animator myAnimator;
};
