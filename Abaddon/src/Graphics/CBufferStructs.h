#pragma once
#include "Bindables/CBuffer.hpp"
#include <DirectXMath.h>

__declspec(align(16))
struct TransformBuffer
{
	DirectX::XMMATRIX myTransformation;
};

__declspec(align(16))
struct DebugBuffer
{
	unsigned int myDebugMode;
	unsigned int myBoneId;
};