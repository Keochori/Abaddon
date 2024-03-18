#include "pch.h"
#include "Scene.h"

void Scene::Init(Input& aInput)
{
	myCamera = std::make_shared<Camera>(aInput);
	myCamera->Init(0.9f, 0.005f);
}

void Scene::Update()
{
	myCamera->Update();

	for (std::shared_ptr<Model> model : myModels)
	{
		model->Update(myCamera);
	}
}

std::shared_ptr<Camera> Scene::GetCamera()
{
	return myCamera;
}

std::vector<std::shared_ptr<Model>> Scene::GetModels()
{
	return myModels;
}

void Scene::AddModel(std::shared_ptr<Model> aModel)
{
	myModels.push_back(aModel);
}
