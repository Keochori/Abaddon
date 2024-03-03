#pragma once
#include "Math/vector4.hpp"

struct Vertex
{
	Vertex() 
	{
		position = { 0,0,0,1 };
		position2 = { 0,0,0,1 };
		color = { 0,0,0,1 };
		color2 = { 0,0,0,1 };
	}

	Vertex(float aX, float aY, float aZ, float aX2, float aY2, float aZ2, float aR, float aG, float aB, float aR2, float aG2, float aB2)
	{ 
		position.x = aX;
		position.y = aY;
		position.z = aZ;
		position.w = 1;

		position2.x = aX2;
		position2.y = aY2;
		position2.z = aZ2;
		position2.w = 1;

		color.x = aR;
		color.y = aG;
		color.z = aB;
		color.w = 1;

		color2.x = aR2;
		color2.y = aG2;
		color2.z = aB2;
		color2.w = 1;
	}

	Vertex(math::vector4<float>& aPosition, math::vector4<float>& aPosition2, math::vector4<float>& aColor, math::vector4<float>& aColor2)
	{ 
		position = aPosition;
		position2 = aPosition2;

		color = aColor;
		color2 = aColor2;
	}

	math::vector4<float> position;
	math::vector4<float> position2;
	math::vector4<float> color;
	math::vector4<float> color2;
};