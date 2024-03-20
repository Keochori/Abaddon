#pragma once
#include "Math/vector3.hpp"

struct Transform
{
	Transform()
	{
		myPosition = { 0,0,0 };
		myRotation = { 0,0,0 };
		myScale = { 1,1,1 };
	}

	math::vector3<float> myPosition;
	math::vector3<float> myRotation;
	math::vector3<float> myScale;
};