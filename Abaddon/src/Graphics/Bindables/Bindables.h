#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "CBuffer.hpp"
#include "SRV.h"
#include "Sampler.h"
#include "Graphics/Animation/AnimationDefines.h"

#include <DirectXMath.h>
#include <unordered_map>

struct Bone
{
	int myId;
	std::string myName;

	DirectX::XMMATRIX myOffsetMatrix;
	DirectX::XMMATRIX myFinalTransformation;
};

struct Skeleton
{
	std::string myName;

	std::unordered_map<std::string, int> myBoneNameToIndexMap;
	std::vector<Bone> myBones;
	int myBoneAmount;

	DirectX::XMMATRIX* GetBoneTransforms()
	{
		DirectX::XMMATRIX boneTransforms[MAX_BONES];

		for (int i = 0; i < 128; i++)
		{
			boneTransforms[i] = DirectX::XMMatrixIdentity();
		}

		for (int i = 0; i < myBoneAmount; i++)
		{
			boneTransforms[i] = myBones[i].myFinalTransformation;
		}

		return boneTransforms;
	}
};

struct ModelData
{
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;

	bool myHasSkeleton;
	Skeleton mySkeleton;
};

struct TextureData
{
	SRV mySRV;
	Sampler mySampler;
};