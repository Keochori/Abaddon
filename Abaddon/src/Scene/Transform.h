#pragma once
#include "Math/vector3.hpp"
#include "Math/vector4.hpp"

struct Transform
{
	Transform()
	{
		myPosition = { 0,0,0 };
		myQuaternionTest = { 0,0,0,1 };
		myScale = { 1,1,1 };
	}

	math::vector3<float> myPosition;
	math::vector3<float> myRotation;
	math::vector4<float> myQuaternionTest;
	math::vector3<float> myScale;
};