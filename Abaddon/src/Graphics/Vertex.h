#pragma once
#include "Math/vector4.hpp"

struct Vertex
{
	Vertex() 
	{
		position = { 0,0,0,1 };
	}

	Vertex(float aX, float aY, float aZ)
	{ 
		position.x = aX;
		position.y = aY;
		position.z = aZ;
		position.w = 1;
	}

	Vertex(math::vector4<float>& aPosition)
	{ 
		position = aPosition;
	}

	math::vector4<float> position;
};