#pragma once
#include "Math/matrix4x4.hpp"
#include <DirectXMath.h>

__declspec(align(16))
struct TestCBuffer 
{
	TestCBuffer(unsigned int aTime, DirectX::XMMATRIX aTransformation)
	{
		myTransformation = aTransformation;
		myTime = aTime;
	}

	DirectX::XMMATRIX myTransformation;
	unsigned int myTime;
};