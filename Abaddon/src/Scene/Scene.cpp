#include "pch.h"
#include "Scene.h"
#include "Graphics/Renderer.h"
#include "Entity.h"
#include "ModelAssetHandler.h"
#include "Components/Components.h"
#include "Scene/Scripts/PlayerMovement.h"

Scene::Scene(std::shared_ptr<Renderer> aRenderer) : myRenderer(aRenderer)
{
}

Scene::~Scene()
{
	myRegistry.view<ScriptComponent>().each([=](entt::entity aEntity, ScriptComponent& aScriptComponent)
		{
			if (aScriptComponent.myInstance)
			{
				aScriptComponent.myInstance->OnDestroy();
			}
		});
}

void Scene::Init()
{
	myCamera = std::make_shared<Camera>();
	myCamera->Init(3.0f, 0.005f);

	ModelAssetHandler::LoadModel(MODELMODEL);
	ModelAssetHandler::LoadAnimation(MODELMODEL);
	ModelAssetHandler::LoadTexture("gremlin.jpg");

	Entity obj = CreateEntity();
	obj.GetComponent<ModelComponent>().myModelName = MODELMODEL;
	obj.GetComponent<ModelComponent>().myTextureName = "gremlin.jpg";
	obj.GetComponent<TransformComponent>().myTransform.myPosition = { 0.0f,0.0f,100.0f };
	//obj.GetComponent<TransformComponent>().myTransform.myScale = { 10.0f,10.0f,10.0f };
}

void Scene::Update()
{
	myCamera->Update();

	auto group = myRegistry.group<TransformComponent>(entt::get<ModelComponent>);
	for (auto entity : group)
	{
		std::tuple<TransformComponent, ModelComponent> object = group.get<TransformComponent, ModelComponent>(entity);

		ModelData& modelData = ModelAssetHandler::GetModelData(std::get<1>(object).myModelName);
		TextureData& textureData = ModelAssetHandler::GetTextureData(std::get<1>(object).myTextureName);
		
		if (!ModelAssetHandler::inited)
		{
			ModelAssetHandler::SetAnim(ModelAssetHandler::GetAnimation(MODELMODEL), &modelData.mySkeleton, MODELMODEL);
		}

		myRenderer->Render(modelData, textureData, std::get<0>(object).myTransform, myCamera);
	}
	ModelAssetHandler::Update();

	myRegistry.view<ScriptComponent>().each([=](entt::entity aEntity, ScriptComponent& aScriptComponent)
		{
			if (!aScriptComponent.myInstance) 
			{
				aScriptComponent.myInstance = aScriptComponent.InitFunction();
				aScriptComponent.myInstance->myEntity = Entity{ aEntity, this };
				aScriptComponent.myInstance->Start();
			}

			aScriptComponent.myInstance->Update();
		});
}

Entity Scene::CreateEntity(std::string aName)
{
	Entity entity = { myRegistry.create(), this };
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<TagComponent>(aName);
	entity.AddComponent<ModelComponent>("gremlin.fbx", "gremlin.jpg");

	return entity;
}

std::shared_ptr<Camera> Scene::GetCamera()
{
	return myCamera;
}
