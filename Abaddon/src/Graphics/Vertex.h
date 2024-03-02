#pragma once
#include "Math/vector4.hpp"

struct Vertex
{
	Vertex() 
	{
		position = { 0,0,0,1 };
		color = { 0,0,0,1 };
	}

	Vertex(float aX, float aY, float aZ, float aR, float aG, float aB) 
	{ 
		position.x = aX;
		position.y = aY;
		position.z = aZ;
		position.w = 1;

		color.x = aR;
		color.y = aG;
		color.z = aB;
		position.w = 1;
	}

	Vertex(math::vector4<float>& aPosition, math::vector4<float>& aColor) 
	{ 
		position = aPosition;

		color = aColor;
	}

	math::vector4<float> position;
	math::vector4<float> color;
};