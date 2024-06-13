#pragma once
#include "Graphics/Bindables/Bindables.h"
#include <unordered_map>
#include "Graphics/Animation/Animator.h"

struct aiNode;

class ModelAssetHandler
{
public:
	static void LoadModel(std::string aModelFileName);
	static void LoadTexture(std::string aTextureFileName);
	static void LoadAnimation(std::string aAnimationFileName);

	static ModelData& GetModelData(std::string aModelFileName);
	static TextureData& GetTextureData(std::string aTextureFileName);
	static Animation& GetAnimation(std::string aAnimationFileName);

private:
	static void CreateBoneHierarchy(aiNode* aNode, Bone* aBone);
	static aiNode* GetPreRotation(aiNode* aNode);
	static std::unordered_map<std::string, ModelData>  myLoadedModels;
	static std::unordered_map<std::string, Animation>  myLoadedAnimations;
	static std::unordered_map<std::string, TextureData>  myLoadedTextures;
	static int currentBoneIndex;
	static std::unordered_map<std::string, int> names;
};

