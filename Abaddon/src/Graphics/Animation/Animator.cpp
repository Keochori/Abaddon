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
	UpdateBoneTransforms(mySkeleton->myRootBone);
	UpdateResultTransforms(mySkeleton->myRootBone, DirectX::XMMatrixIdentity());

	myTimer += Timer::GetInstance().GetDeltaTime() * 10;

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

void Animator::UpdateBoneTransforms(Bone& aBone)
{
	if (myCurrentAnim.myKeyFrames[myCurrentFrame].myPose.find(aBone.myName) != myCurrentAnim.myKeyFrames[myCurrentFrame].myPose.end())
	{
		Transform bonePose = myCurrentAnim.myKeyFrames[myCurrentFrame].myPose.at(aBone.myName);
		aBone.myAnimatedTransform = TransformToMatrix(bonePose);
	}

	for (Bone& childBone : aBone.myChildren)
	{
		UpdateBoneTransforms(childBone);
	}
}

void Animator::UpdateResultTransforms(Bone& aBone, DirectX::XMMATRIX aParentTransform)
{
	DirectX::XMMATRIX animTransform = aBone.myAnimatedTransform;
	DirectX::XMMATRIX transposed = DirectX::XMMatrixTranspose(aBone.myAnimatedTransform);
	DirectX::XMMATRIX multiplied = DirectX::XMMatrixMultiply(aParentTransform, DirectX::XMMatrixTranspose(aBone.myAnimatedTransform));
	DirectX::XMMATRIX inverse = DirectX::XMMatrixMultiply(multiplied, aBone.myInverseBindTransform);

	DirectX::XMMATRIX currentTransform = DirectX::XMMatrixMultiply(aParentTransform, DirectX::XMMatrixTranspose(aBone.myAnimatedTransform));

	for (Bone& childBone : aBone.myChildren)
	{
		UpdateResultTransforms(childBone, currentTransform);
	}

	currentTransform = DirectX::XMMatrixMultiply(currentTransform, aBone.myInverseBindTransform);

	aBone.myResultTranform = currentTransform;
}

DirectX::XMMATRIX Animator::TransformToMatrix(Transform aTransform)
{
	DirectX::XMMATRIX pos = DirectX::XMMatrixTranslation(aTransform.myPosition.x, aTransform.myPosition.y, aTransform.myPosition.z);
	DirectX::XMMATRIX scl = DirectX::XMMatrixScaling(aTransform.myScale.x, aTransform.myScale.y, aTransform.myScale.z);
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(aTransform.myQuaternionTest.x, aTransform.myQuaternionTest.y, aTransform.myQuaternionTest.z, aTransform.myQuaternionTest.w));
	//DirectX::XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYaw(aTransform.myRotation.x, aTransform.myRotation.y, aTransform.myRotation.z);

	DirectX::XMMATRIX r1 = DirectX::XMMatrixMultiply(rot, scl);
	DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(pos, r1);

	DirectX::XMMATRIX result2 = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(aTransform.myQuaternionTest.x, aTransform.myQuaternionTest.y, aTransform.myQuaternionTest.z, aTransform.myQuaternionTest.w)) *
		DirectX::XMMatrixTranslation(aTransform.myPosition.x, aTransform.myPosition.y, aTransform.myPosition.z) *
		DirectX::XMMatrixScaling(aTransform.myScale.x, aTransform.myScale.y, aTransform.myScale.z);

	return result;
}
