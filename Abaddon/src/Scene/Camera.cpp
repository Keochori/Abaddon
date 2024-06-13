#include "pch.h"
#include "Camera.h"

Camera::Camera()
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
	if (Input::GetInstance().IsMouseButtonDown((int)eKeys::MOUSERBUTTON))
	{
		myCamPitch += Input::GetInstance().GetMouseDelta().y * myRotationSpeed;
		myCamYaw += Input::GetInstance().GetMouseDelta().x * myRotationSpeed;
	}

	// Speed
	myMovementSpeed -= ((Input::GetInstance().myTentativeScrollDelta < 0) ? -1 : (Input::GetInstance().myTentativeScrollDelta > 0)) * 0.125f;
	if (myMovementSpeed < 0.1f) myMovementSpeed = 0.1f;
	else if (myMovementSpeed > 3.0f) myMovementSpeed = 3.0f;

	// Keyboard
	if (Input::GetInstance().IsKeyDown((int)eKeys::W))
	{
		myInputBackForward += myMovementSpeed;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::S))
	{
		myInputBackForward -= myMovementSpeed;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::A))
	{
		myInputLeftRight -= myMovementSpeed;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::D))
	{
		myInputLeftRight += myMovementSpeed;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::Q))
	{
		myInputUpDown -= myMovementSpeed;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::E))
	{
		myInputUpDown += myMovementSpeed;
	}
}
