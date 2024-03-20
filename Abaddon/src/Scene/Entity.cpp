#include "pch.h"
#include "Entity.h"

Entity::Entity(entt::entity aHandle, Scene* aScene) : myEntityHandle(aHandle), myScene(aScene)
{
}

Entity::~Entity()
{
	myScene = nullptr;
	delete myScene;
}
