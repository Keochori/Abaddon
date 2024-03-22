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
	bool twoDir = false;
	if (Input::GetInstance().IsKeyDown((int)eKeys::UP) && Input::GetInstance().IsKeyDown((int)eKeys::LEFT))
	{
		GetComponent<TransformComponent>().myTransform.myPosition += {-mySpeed, 0, mySpeed};
		GetComponent<TransformComponent>().myTransform.myRotation = { 0, -45, 0 };
		twoDir = true;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::UP) && Input::GetInstance().IsKeyDown((int)eKeys::RIGHT))
	{
		GetComponent<TransformComponent>().myTransform.myPosition += {mySpeed, 0, mySpeed};
		GetComponent<TransformComponent>().myTransform.myRotation = { 0, 45, 0 };
		twoDir = true;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::DOWN) && Input::GetInstance().IsKeyDown((int)eKeys::LEFT))
	{
		GetComponent<TransformComponent>().myTransform.myPosition += {-mySpeed, 0, -mySpeed};
		GetComponent<TransformComponent>().myTransform.myRotation = { 0, -135, 0 };
		twoDir = true;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::DOWN) && Input::GetInstance().IsKeyDown((int)eKeys::RIGHT))
	{
		GetComponent<TransformComponent>().myTransform.myPosition += {mySpeed, 0, -mySpeed};
		GetComponent<TransformComponent>().myTransform.myRotation = { 0, 135, 0 };
		twoDir = true;
	}

	if (!twoDir)
	{
		if (Input::GetInstance().IsKeyDown((int)eKeys::UP))
		{
			GetComponent<TransformComponent>().myTransform.myPosition += {0, 0, mySpeed};
			GetComponent<TransformComponent>().myTransform.myRotation = { 0, 0, 0 };
		}
		if (Input::GetInstance().IsKeyDown((int)eKeys::LEFT))
		{
			GetComponent<TransformComponent>().myTransform.myPosition += {-mySpeed, 0, 0};
			GetComponent<TransformComponent>().myTransform.myRotation = { 0, -90.0f, 0 };
		}
		if (Input::GetInstance().IsKeyDown((int)eKeys::DOWN))
		{
			GetComponent<TransformComponent>().myTransform.myPosition += {0, 0, -mySpeed};
			GetComponent<TransformComponent>().myTransform.myRotation = { 0, 180, 0 };
		}
		if (Input::GetInstance().IsKeyDown((int)eKeys::RIGHT))
		{
			GetComponent<TransformComponent>().myTransform.myPosition += {mySpeed, 0, 0};
			GetComponent<TransformComponent>().myTransform.myRotation = { 0, 90.0f, 0 };
		}
	}
}
