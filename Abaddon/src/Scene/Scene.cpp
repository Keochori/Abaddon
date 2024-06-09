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
	ModelAssetHandler::LoadAnimation("gremlin@run.fbx");
	ModelAssetHandler::LoadTexture("gremlin.jpg");

	//ModelAssetHandler::LoadModel("Angel.fbx");
	//ModelAssetHandler::LoadAnimation("Angel.fbx");
	//ModelAssetHandler::LoadTexture("chest.jpg");

	//Entity obj = CreateEntity();
	//obj.AddComponent<ScriptComponent>().Bind<PlayerMovement>();
	//obj.GetComponent<TransformComponent>().myTransform.myPosition = { 1000.3f,0.3f,0.3f };

	Entity obj2 = CreateEntity();
	obj2.GetComponent<ModelComponent>().myModelName = "gremlin.fbx";
	obj2.GetComponent<ModelComponent>().myTextureName = "gremlin.jpg";
	obj2.GetComponent<ModelComponent>().myAnimationName = "gremlin@run.fbx";
	//obj2.GetComponent<TransformComponent>().myTransform.myScale = { 0.3f,0.3f,0.3f };
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

		if (!myAnimator.inited)
			myAnimator.SetAnim(ModelAssetHandler::GetAnimation(std::get<1>(object).myAnimationName), &modelData.mySkeleton);

		myAnimator.Update();

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
