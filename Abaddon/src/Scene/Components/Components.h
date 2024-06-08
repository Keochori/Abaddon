#pragma once
#include "Scene/Transform.h"
#include "Scene/Scripts/Script.h"
#include <functional>
#include <string>

struct TransformComponent
{
	Transform myTransform;
};

struct TagComponent
{
	TagComponent(std::string aTag)
	{
		myTag = aTag;
	}

	std::string myTag;
};

struct ScriptComponent
{
	Script* myInstance = nullptr;

	Script*(*InitFunction)();
	void(*DestroyFunction)(ScriptComponent*);

	template<typename T>
	void Bind()
	{
		InitFunction = []() { return static_cast<Script*>(new T()); };
		DestroyFunction = [](ScriptComponent* aScriptComponent) { delete aScriptComponent->myInstance; aScriptComponent->myInstance = nullptr; };
	}
};

struct ModelComponent
{
	ModelComponent(std::string aModelName, std::string aTextureName)
	{
		myModelName = aModelName;
		myTextureName = aTextureName;
	}

	std::string myModelName;
	std::string myTextureName;
	std::string myAnimationName;
};

