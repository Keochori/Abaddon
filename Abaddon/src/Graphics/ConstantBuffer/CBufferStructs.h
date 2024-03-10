#pragma once
#include "CBuffer.h"
#include "Math/matrix4x4.hpp"
#include <DirectXMath.h>

__declspec(align(16))
struct TransformBuffer
{
	DirectX::XMMATRIX myTransformation;
};

struct FaceColorsBuffer
{
	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};
	
	Color myFaceColors[6];
};