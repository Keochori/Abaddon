#pragma once
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"
#include "Tools/Input.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera() = default;

	void Init(float aCameraSpeed, float aRotationSpeed, bool aFirstPerson = false);
	void Update();
	XMMATRIX GetMatrix();

private:
	void CalculateMatrix();
	void UpdateInput();

	bool myFirstPerson;
	float myMovementSpeed;
	float myRotationSpeed;

	XMMATRIX myCameraMatrix;

private:
	// Variables used to calculate Camera Matrix
	XMVECTOR myDefaultForward;
	XMVECTOR myDefaultRight;
	XMMATRIX myCamRotationMatrix;

	XMVECTOR myCamForward;
	XMVECTOR myCamRight;

	XMVECTOR myCamPosition;
	XMVECTOR myCamTarget;
	XMVECTOR myCamUp;

	float myInputLeftRight = 0.0f;
	float myInputBackForward = 0.0f;
	float myInputUpDown = 0.0f;
	float myCamYaw = 0.0f;
	float myCamPitch = 0.0f;
};

