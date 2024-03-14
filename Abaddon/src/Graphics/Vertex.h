#pragma once
#include "Math/vectors.h"

struct Vertex
{
	Vertex() 
	{
		myPosition = { 0,0,0,1 };
		myTexcoord = { 0.0f,0.0f };
	}

	Vertex(float aX, float aY, float aZ, float aU, float aV)
	{ 
		myPosition.x = aX;
		myPosition.y = aY;
		myPosition.z = aZ;
		myPosition.w = 1;

		myTexcoord.x = aU;
		myTexcoord.y = aV;
	}

	Vertex(math::vector3<float>& aPosition, math::vector2<float> aTexcoord)
	{ 
		myPosition.x = aPosition.x;
		myPosition.y = aPosition.y;
		myPosition.z = aPosition.z;
		myPosition.w = 1;

		myTexcoord.x = aTexcoord.x;
		myTexcoord.y = aTexcoord.y;
	}

	math::vector4<float> myPosition;
	math::vector2<float> myTexcoord;
};