#pragma once
#include "Graphics/Bindables/Bindables.h"
#include <unordered_map>
#include "Scene/Transform.h"
#include "Tools/Timer.h"

#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

struct aiNode;
struct aiScene;

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

class ModelAssetHandler
{
public:
	static void LoadModel(std::string aModelFileName);
	static void LoadAnimation(std::string aAnimationFileName);
	static void LoadTexture(std::string aTextureFileName);

	static ModelData& GetModelData(std::string aModelFileName);
	static Animation& GetAnimation(std::string aAnimationFileName);
	static TextureData& GetTextureData(std::string aTextureFileName);

	static void ReadBoneHierarchy(aiNode* aNode, DirectX::XMMATRIX aParentTransform, Skeleton& aSkeleton);

	// ANIM
	static void SetAnim(Animation aAnimation, Skeleton* aSkeleton, std::string aModelName);
	static void Update();
	static void UpdateTransform(aiNode* aNode, DirectX::XMMATRIX aParentTransform, Skeleton& aSkeleton);

	static Animation myCurrentAnim;
	static Skeleton* mySkeleton;
	static float myTimer;
	static int myCurrentFrame;
	static bool inited;
	static std::string myModelName;
	static Assimp::Importer importer;
private:
	static std::unordered_map<std::string, ModelData>  myLoadedModels;
	static std::unordered_map<std::string, Animation>  myLoadedAnimations;
	static std::unordered_map<std::string, TextureData>  myLoadedTextures;
	static const aiScene* myData;
};

