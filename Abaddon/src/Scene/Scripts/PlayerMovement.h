#pragma once
#include "Scene/Components/Components.h"
#include "Log.h"

class PlayerMovement : public Script
{
public:
	PlayerMovement() = default;

	void Start();
	void OnDestroy();
	void Update();
	
private:
	float mySpeed;
};