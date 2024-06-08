#pragma once
#include "Bindables/CBuffer.hpp"
#include <DirectXMath.h>

__declspec(align(16))
struct TransformBuffer
{
	DirectX::XMMATRIX myTransformation;
	DirectX::XMMATRIX myBoneData[128];
};