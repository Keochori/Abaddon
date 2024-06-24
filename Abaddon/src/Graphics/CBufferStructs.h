#pragma once
#include "Bindables/CBuffer.hpp"
#include <DirectXMath.h>

__declspec(align(16))
struct TransformBuffer
{
	DirectX::XMMATRIX myTransformationMatrix;
	DirectX::XMMATRIX myViewMatrix;
	DirectX::XMMATRIX myProjectionMatrix;
};

__declspec(align(16))
struct DebugBuffer
{
	unsigned int myDebugMode;
	unsigned int myBoneId;
};