#pragma once
#include "Math/vectors.h"

#define MAX_BONES 4

struct Vertex
{
	Vertex() 
	{
		myPosition = { 0,0,0,1 };
		myTexcoord = { 0.0f,0.0f };

		myBoneIDs[0] = 0;
		myBoneIDs[1] = 0;
		myBoneIDs[2] = 0;
		myBoneIDs[3] = 0;

		myBoneWeights[0] = 0;
		myBoneWeights[1] = 0;
		myBoneWeights[2] = 0;
		myBoneWeights[3] = 0;
	}

	Vertex(float aX, float aY, float aZ, float aU, float aV)
	{ 
		myPosition.x = aX;
		myPosition.y = aY;
		myPosition.z = aZ;
		myPosition.w = 1;

		myTexcoord.x = aU;
		myTexcoord.y = aV;

		myBoneIDs[0] = 0;
		myBoneIDs[1] = 0;
		myBoneIDs[2] = 0;
		myBoneIDs[3] = 0;

		myBoneWeights[0] = 0;
		myBoneWeights[1] = 0;
		myBoneWeights[2] = 0;
		myBoneWeights[3] = 0;
	}

	Vertex(math::vector3<float>& aPosition, math::vector2<float> aTexcoord)
	{ 
		myPosition.x = aPosition.x;
		myPosition.y = aPosition.y;
		myPosition.z = aPosition.z;
		myPosition.w = 1;

		myTexcoord.x = aTexcoord.x;
		myTexcoord.y = aTexcoord.y;

		myBoneIDs[0] = 0;
		myBoneIDs[1] = 0;
		myBoneIDs[2] = 0;
		myBoneIDs[3] = 0;

		myBoneWeights[0] = 0;
		myBoneWeights[1] = 0;
		myBoneWeights[2] = 0;
		myBoneWeights[3] = 0;
	}

	math::vector4<float> myPosition;
	math::vector2<float> myTexcoord;
	unsigned int myBoneIDs[MAX_BONES];
	float myBoneWeights[MAX_BONES];
};