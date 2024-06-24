#pragma once
#include "Graphics/Bindables/Bindables.h"
#include <unordered_map>

struct aiNode;

class ModelAssetHandler
{
public:
	static void LoadModel(std::string aModelFileName);
	static void LoadTexture(std::string aTextureFileName);

	static ModelData& GetModelData(std::string aModelFileName);
	static TextureData& GetTextureData(std::string aTextureFileName);

private:
	static int currentBoneIndex;
	static void CreateBoneHierarchy(aiNode* aNode, Bone& aBone, std::vector<std::string> aValidNames);

	static std::unordered_map<std::string, ModelData>  myLoadedModels;
	static std::unordered_map<std::string, TextureData>  myLoadedTextures;
};

