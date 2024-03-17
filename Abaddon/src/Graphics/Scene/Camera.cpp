#include "pch.h"
#include "Camera.h"

Camera::Camera(Input& aInput) : myInput(aInput)
{
}

void Camera::Init(float aMovementSpeed, float aRotationSpeed, bool aFirstPerson)
{
	myFirstPerson = aFirstPerson;

	myMovementSpeed = aMovementSpeed;
	myRotationSpeed = aRotationSpeed;

	myDefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	myDefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	myCamForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	myCamRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	myCamTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	myCamPosition = DirectX::XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	myCamUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	CalculateMatrix();
}

void Camera::Update()
{
	UpdateInput();
	CalculateMatrix();
}

XMMATRIX Camera::GetMatrix()
{
	return myCameraMatrix;
}

void Camera::CalculateMatrix()
{
	// Reset values
	myCamForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	myCamRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	myCamRotationMatrix = XMMatrixIdentity();

	// Calculates new forward direction based on cameras rotation
	myCamRotationMatrix = XMMatrixRotationRollPitchYaw(myCamPitch, myCamYaw, 0);
	myCamTarget = XMVector3TransformCoord(myDefaultForward, myCamRotationMatrix);
	myCamTarget = XMVector3Normalize(myCamTarget);

	// Calculate new left, right and up direction based on cameras rotation
	if (myFirstPerson) 
	{
		// First Person camera
		XMMATRIX RotateYTempMatrix;
		RotateYTempMatrix = XMMatrixRotationY(myCamYaw);

		myCamRight = XMVector3TransformCoord(myDefaultRight, RotateYTempMatrix);
		myCamForward = XMVector3TransformCoord(myDefaultForward, RotateYTempMatrix);
		myCamUp = XMVector3TransformCoord(myCamUp, RotateYTempMatrix);
	}
	else
	{
		// Free-Look camera
		myCamRight = XMVector3TransformCoord(myDefaultRight, myCamRotationMatrix);
		myCamForward = XMVector3TransformCoord(myDefaultForward, myCamRotationMatrix);
		myCamUp = XMVector3Cross(myCamForward, myCamRight);
	}

	// Update position
	myCamPosition = XMVectorAdd(myCamPosition, XMVectorScale(myCamRight, myInputLeftRight));
	myCamPosition = XMVectorAdd(myCamPosition, XMVectorScale(myCamForward, myInputBackForward));
	myCamPosition = XMVectorAdd(myCamPosition, XMVECTOR({ 0.0f, myInputUpDown, 0.0f, 0.0f }));

	myInputLeftRight = 0.0f;
	myInputBackForward = 0.0f;
	myInputUpDown = 0.0f;

	// Calculate final target
	myCamTarget = XMVectorAdd(myCamPosition, myCamTarget);

	// Set camera matrix
	myCameraMatrix = XMMatrixLookAtLH(myCamPosition, myCamTarget, myCamUp);
}

void Camera::UpdateInput()
{
	if (myInput.IsMouseButtonDown((int)eKeys::MOUSERBUTTON))
	{
		myCamPitch += myInput.GetMouseDelta().y * myRotationSpeed;
		myCamYaw += myInput.GetMouseDelta().x * myRotationSpeed;
	}

	// Keyboard
	if (myInput.IsKeyDown((int)eKeys::W))
	{
		myInputBackForward += myMovementSpeed;
	}
	if (myInput.IsKeyDown((int)eKeys::S))
	{
		myInputBackForward -= myMovementSpeed;
	}
	if (myInput.IsKeyDown((int)eKeys::A))
	{
		myInputLeftRight -= myMovementSpeed;
	}
	if (myInput.IsKeyDown((int)eKeys::D))
	{
		myInputLeftRight += myMovementSpeed;
	}
	if (myInput.IsKeyDown((int)eKeys::Q))
	{
		myInputUpDown -= myMovementSpeed;
	}
	if (myInput.IsKeyDown((int)eKeys::E))
	{
		myInputUpDown += myMovementSpeed;
	}
}
