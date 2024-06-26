#include "pch.h"
#include "Animator.h"

void Animator::SetAnim(Animation aAnimation, Skeleton* aSkeleton)
{
	myCurrentAnim = aAnimation;
	mySkeleton = aSkeleton;
	inited = true;
}

void Animator::Update()
{
	myTimer += Timer::GetInstance().GetDeltaTime() * 10;

	UpdateTransforms();

	if (myTimer > myCurrentAnim.myLength)
	{
		myTimer = 0;
		myCurrentFrame = 0;
	}
	else
	{
		if (myTimer > myCurrentAnim.myKeyFrames[myCurrentFrame + 1].myTimeStamp)
		{
			myCurrentFrame++;
		}
	}

	LOG(std::to_string(myCurrentFrame));
}

void Animator::UpdateTransforms()
{
	
}

DirectX::XMMATRIX Animator::TransformToMatrix(Transform aTransform)
{
	DirectX::XMMATRIX result = 
	DirectX::XMMatrixTranslation(aTransform.myPosition.x, aTransform.myPosition.y, aTransform.myPosition.z) *
	DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(aTransform.myQuaternion.x, aTransform.myQuaternion.y, aTransform.myQuaternion.z, aTransform.myQuaternion.w)) *
	DirectX::XMMatrixScaling(aTransform.myScale.x, aTransform.myScale.y, aTransform.myScale.z);

	return result;
}
