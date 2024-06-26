#pragma once
#include "Math/vectors.h"
#include "Animation/AnimationDefines.h"

struct Vertex
{
	Vertex() 
	{
		myPosition = { 0,0,0,1 };
		myTexcoord = { 0.0f,0.0f };

		for (int i = 0; i < MAX_WEIGHTS; i++)
		{
			myBoneIDs[i] = -1;
			myBoneWeights[i] = 0;
		}
	}

	Vertex(float aX, float aY, float aZ, float aU, float aV)
	{ 
		myPosition.x = aX;
		myPosition.y = aY;
		myPosition.z = aZ;
		myPosition.w = 1;

		myTexcoord.x = aU;
		myTexcoord.y = aV;

		for (int i = 0; i < MAX_WEIGHTS; i++)
		{
			myBoneIDs[i] = -1;
			myBoneWeights[i] = 0;
		}
	}

	Vertex(math::vector3<float>& aPosition, math::vector2<float> aTexcoord)
	{ 
		myPosition.x = aPosition.x;
		myPosition.y = aPosition.y;
		myPosition.z = aPosition.z;
		myPosition.w = 1;

		myTexcoord.x = aTexcoord.x;
		myTexcoord.y = aTexcoord.y;

		for (int i = 0; i < MAX_WEIGHTS; i++)
		{
			myBoneIDs[i] = -1;
			myBoneWeights[i] = 0;
		}
	}

	math::vector4<float> myPosition;
	math::vector2<float> myTexcoord;
	int myBoneIDs[MAX_WEIGHTS];
	float myBoneWeights[MAX_WEIGHTS];
};