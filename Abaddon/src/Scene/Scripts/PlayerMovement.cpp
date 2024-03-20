#include "pch.h"
#include "PlayerMovement.h"
#include "Tools/Input.h"

void PlayerMovement::Start()
{
	mySpeed = 1.0f;
}

void PlayerMovement::OnDestroy()
{
	LOG("Script Destroyed");
}

void PlayerMovement::Update()
{
	if (Input::GetInstance().IsKeyDown((int)eKeys::UP))
		GetComponent<TransformComponent>().myTransform.myPosition += {0, 0, mySpeed};
	if (Input::GetInstance().IsKeyDown((int)eKeys::LEFT))
		GetComponent<TransformComponent>().myTransform.myPosition += {-mySpeed, 0, 0};
	if (Input::GetInstance().IsKeyDown((int)eKeys::DOWN))
		GetComponent<TransformComponent>().myTransform.myPosition += {0, 0, -mySpeed};
	if (Input::GetInstance().IsKeyDown((int)eKeys::RIGHT))
		GetComponent<TransformComponent>().myTransform.myPosition += {mySpeed, 0, 0};
}
