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
	myCamera->Init(0.9f, 0.005f);

	ModelAssetHandler::LoadModel("gremlin.fbx");
	ModelAssetHandler::LoadTexture("gremlin.jpg");
	ModelAssetHandler::LoadModel("chest.fbx");
	ModelAssetHandler::LoadTexture("chest.jpg");

	Entity obj = CreateEntity();
	obj.AddComponent<ScriptComponent>().Bind<PlayerMovement>();

	Entity obj2 = CreateEntity();
	obj2.GetComponent<ModelComponent>().myModelName = "chest.fbx";
	obj2.GetComponent<ModelComponent>().myTextureName = "chest.jpg";
	obj2.GetComponent<TransformComponent>().myTransform.myScale = { 0.3f,0.3f,0.3f };
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
		myRenderer->Render(modelData, textureData, std::get<0>(object).myTransform, myCamera);
	}

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
