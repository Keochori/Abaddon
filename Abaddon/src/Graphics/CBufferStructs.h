#pragma once
#include "Bindables/CBuffer.hpp"
#include "Graphics/Animation/AnimationDefines.h"
#include <DirectXMath.h>

__declspec(align(16))
struct TransformBuffer
{
	DirectX::XMMATRIX myTransformationMatrix;
	DirectX::XMMATRIX myViewMatrix;
	DirectX::XMMATRIX myProjectionMatrix;
	DirectX::XMMATRIX myBoneTransforms[MAX_BONES];
};

__declspec(align(16))
struct DebugBuffer
{
	unsigned int myDebugMode;
	unsigned int myBoneId;
};