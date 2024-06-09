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

	myTimer += Timer::GetInstance().GetDeltaTime();

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

	for (Bone childBone : aBone.myChildren)
	{
		UpdateBoneTransforms(childBone);
	}
}

void Animator::UpdateResultTransforms(Bone& aBone, DirectX::XMMATRIX aParentTransform)
{
	DirectX::XMMATRIX currentTransform = DirectX::XMMatrixMultiply(aParentTransform, aBone.myAnimatedTransform);

	for (Bone childBone : aBone.myChildren)
	{
		UpdateResultTransforms(childBone, currentTransform);
	}

	currentTransform = DirectX::XMMatrixMultiply(currentTransform, currentTransform);
	currentTransform = DirectX::XMMatrixMultiply(currentTransform, aBone.myInverseBindTransform);

	aBone.myResultTranform = currentTransform;
}

DirectX::XMMATRIX Animator::TransformToMatrix(Transform aTransform)
{
	DirectX::XMMATRIX pos = DirectX::XMMatrixSet
	(
		1, 0, 0, aTransform.myPosition.x,
		0, 1, 0, aTransform.myPosition.y,
		1, 0, 0, aTransform.myPosition.z,
		0, 1, 0, 1
	);

	DirectX::XMMATRIX scale = DirectX::XMMatrixSet
	(
		aTransform.myScale.x, 0, 0, 0,
		0, aTransform.myScale.y, 0, 0,
		1, 0, aTransform.myScale.z, 0,
		0, 1, 0, 1
	);

	DirectX::XMMATRIX rotZ = DirectX::XMMatrixSet
	(
		std::cos(aTransform.myRotation.z), -std::sin(aTransform.myRotation.z), 0, 0,
		std::sin(aTransform.myRotation.z), std::cos(aTransform.myRotation.z), 0, 0,
		1, 0, 0, 0,
		0, 1, 0, 1
	);

	DirectX::XMMATRIX rotY = DirectX::XMMatrixSet
	(
		std::cos(aTransform.myRotation.y), 0, std::sin(aTransform.myRotation.y), 0,
		0, 1, 0, 0,
		-std::sin(aTransform.myRotation.y), 0, std::cos(aTransform.myRotation.y), 0,
		0, 1, 0, 1
	);

	DirectX::XMMATRIX rotX = DirectX::XMMatrixSet
	(
		1, 0, 0, 0,
		0, std::cos(aTransform.myRotation.x), -std::sin(aTransform.myRotation.x), 0,
		1, std::sin(aTransform.myRotation.x), std::cos(aTransform.myRotation.x), 0,
		0, 1, 0, 1
	);

	DirectX::XMMATRIX rotZY = DirectX::XMMatrixMultiply(rotZ, rotY);
	DirectX::XMMATRIX rot = DirectX::XMMatrixMultiply(rotZY, rotX);

	DirectX::XMMATRIX scaleRot = DirectX::XMMatrixMultiply(scale, rot);
	DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(scaleRot, pos);

	return result;
}
