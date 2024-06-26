#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "Math/vectors.h"
#include <string>
#include "Scene/Transform.h"
#include "Tools/Timer.h"
#include "Graphics/Bindables/Bindables.h"

#include <DirectXMath.h>

struct KeyFrame
{
	float myTimeStamp;
	std::unordered_map<std::string, Transform> myPose;
};

struct Animation
{
	float myLength;
	std::vector<KeyFrame> myKeyFrames;
};

class Animator
{
public:
	void SetAnim(Animation aAnimation, Skeleton* aSkeleton);
	void Update();
	void UpdateTransforms();
	DirectX::XMMATRIX TransformToMatrix(Transform aTransform);
	bool inited = false;

	Animation myCurrentAnim;
private:
	Skeleton* mySkeleton;
	float myTimer = 0;
	int myCurrentFrame = 0;

};

