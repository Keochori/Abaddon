#pragma once
#include "Camera.h"
#include "Model.h"

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	void Init(Input& aInput);
	void Update();

	void AddModel(std::shared_ptr<Model> aModel);

	std::shared_ptr<Camera> GetCamera();
	std::vector<std::shared_ptr<Model>> GetModels();

private:
	std::shared_ptr<Camera> myCamera;
	std::vector<std::shared_ptr<Model>> myModels;
};

