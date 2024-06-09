#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "CBuffer.hpp"
#include "SRV.h"
#include "Sampler.h"

#include <DirectXMath.h>

struct Bone
{
	int myId;
	std::string myName;

	std::vector<Bone> myChildren;

	DirectX::XMMATRIX myAnimatedTransform;
	DirectX::XMMATRIX myResultTranform;
	DirectX::XMMATRIX myInverseBindTransform;
};

struct Skeleton
{
	std::string myName;

	Bone myRootBone;
	int myBoneAmount;
};

struct ModelData
{
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;

	Skeleton mySkeleton;

	DirectX::XMMATRIX boneTransforms[128];

	DirectX::XMMATRIX* GetBoneTransforms()
	{
		addBoneToArray(mySkeleton.myRootBone);

		return boneTransforms;
	}

	void addBoneToArray(Bone aBone)
	{
		boneTransforms[aBone.myId] = aBone.myResultTranform;

		for (Bone childBone : aBone.myChildren)
		{
			addBoneToArray(childBone);
		}
	}
};

struct TextureData
{
	SRV mySRV;
	Sampler mySampler;
};