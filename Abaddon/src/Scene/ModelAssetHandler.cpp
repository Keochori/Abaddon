#include "pch.h"
#include "ModelAssetHandler.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include <DirectXMath.h>

std::unordered_map<std::string, ModelData> ModelAssetHandler::myLoadedModels;
std::unordered_map<std::string, Animation> ModelAssetHandler::myLoadedAnimations;
std::unordered_map<std::string, TextureData> ModelAssetHandler::myLoadedTextures;
std::unordered_map<std::string, int> ModelAssetHandler::names;

void ModelAssetHandler::LoadModel(std::string aModelFileName)
{
	// Check if it's already loaded
	if (myLoadedModels.find(aModelFileName) != myLoadedModels.end())
	{
		LOG_WARNING("Model has already been loaded.");
		Assert(false);
		return;
	}

	// Load file
	Assimp::Importer importer;
	auto loadedData = importer.ReadFile("Models/" + aModelFileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights);
	if (!loadedData)
	{
		LOG_ERROR("Failed to load model '" + aModelFileName + "'");
		Assert(false);
	}

	// Vertex Buffer & Index Buffer list
	ModelData mdlData;
	std::vector<Vertex> vertexList;
	std::vector<unsigned short> indexList;

	for (int mIndex = 0; mIndex < loadedData->mNumMeshes; mIndex++)
	{
		auto mesh = loadedData->mMeshes[mIndex];
		int indexOffset = vertexList.size();

		for (int vIndex = 0; vIndex < mesh->mNumVertices; vIndex++)
		{
			auto vertex = mesh->mVertices[vIndex];

			aiVector3D* textureCoords = mesh->mTextureCoords[0];
			if (textureCoords)
			{
				vertexList.push_back({ vertex.x, vertex.y, vertex.z, mesh->mTextureCoords[0][vIndex].x, 1.0f - mesh->mTextureCoords[0][vIndex].y });
			}
			else
			{
				vertexList.push_back({ vertex.x, vertex.y, vertex.z, 0, 0 });
			}
		}

		for (int fIndex = 0; fIndex < mesh->mNumFaces; fIndex++)
		{
			auto face = mesh->mFaces[fIndex];

			indexList.push_back(face.mIndices[0] + indexOffset);
			indexList.push_back(face.mIndices[1] + indexOffset);
			indexList.push_back(face.mIndices[2] + indexOffset);
		}

		// Skeleton
		if (mesh->HasBones())
		{
			Skeleton* skeleton = &mdlData.mySkeleton;
			skeleton->myName = aModelFileName + ".skeleton";
			skeleton->myBoneAmount = mesh->mNumBones;

			skeleton->myRootBone.myId = 0;
			skeleton->myRootBone.myName = mesh->mBones[0]->mNode->mParent->mName.C_Str();
			skeleton->myRootBone.myInverseBindTransform = DirectX::XMMatrixIdentity();
			skeleton->myRootBone.myAnimatedTransform = DirectX::XMMatrixIdentity();
			skeleton->myRootBone.myResultTranform = DirectX::XMMatrixIdentity();

			for (int i = 0; i < mesh->mNumBones; i++)
			{
				names.insert({ mesh->mBones[i]->mName.C_Str(), 0 });
			}

			CreateBoneHierarchy(mesh->mBones[0]->mNode->mParent, &skeleton->myRootBone);

			// Vertex bone & weight data
			for (int bIndex = 0; bIndex < mesh->mNumBones; bIndex++)
			{
				auto bone = mesh->mBones[bIndex];

				for (int wIndex = 0; wIndex < bone->mNumWeights; wIndex++)
				{
					auto weight = bone->mWeights[wIndex];

					Vertex& vertex = vertexList[weight.mVertexId];
					for (int vBoneId = 0; vBoneId < MAX_BONES; vBoneId++)
					{
						unsigned int currentBoneValue = vertex.myBoneIDs[vBoneId];

						if (currentBoneValue == 0)
						{
							vertex.myBoneIDs[vBoneId] = bIndex + 1;
							vertex.myBoneWeights[vBoneId] = weight.mWeight;
							break;
						} 
					}
				}
			}

			skeleton = nullptr;
			delete skeleton;
		}
	}

	// Create Input Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "BONEIDS" , 0,DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BONEWEIGHTS", 0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

	// Init
	mdlData.myVertexBuffer.Init(vertexList);
	mdlData.myIndexBuffer.Init(indexList);
	mdlData.myInputLayout.Init(inputLayout, "VertexShader_vs.cso");

	// Add to loaded model list
	myLoadedModels.insert({ aModelFileName, mdlData });
}

void ModelAssetHandler::LoadAnimation(std::string aAnimationFileName)
{
	// Check if it's already loaded
	if (myLoadedAnimations.find(aAnimationFileName) != myLoadedAnimations.end())
	{
		LOG_WARNING("Animation has already been loaded.");
		Assert(false);
		return;
	}

	// Load file
	Assimp::Importer importer;
	auto loadedData = importer.ReadFile("Animations/" + aAnimationFileName, 0);
	if (!loadedData)
	{
		LOG_ERROR("Failed to load animation '" + aAnimationFileName + "'");
		Assert(false);
	}

	Animation anim;
	anim.myLength = loadedData->mAnimations[0]->mDuration;

	aiAnimation* currentAnim = loadedData->mAnimations[0];
	int currentFrame = 0;
	int lastFrame = loadedData->mAnimations[0]->mChannels[0]->mNumPositionKeys;

	while (currentFrame < lastFrame)
	{
		KeyFrame newFrame;
		newFrame.myTimeStamp = currentAnim->mChannels[0]->mPositionKeys[currentFrame].mTime;

		for (int chnlIndex = 0; chnlIndex < currentAnim->mNumChannels; chnlIndex++)
		{
			aiNodeAnim* channel = currentAnim->mChannels[chnlIndex];

			Transform pose;
			pose.myPosition.x = channel->mPositionKeys[currentFrame].mValue.x;
			pose.myPosition.y = channel->mPositionKeys[currentFrame].mValue.y;
			pose.myPosition.z = channel->mPositionKeys[currentFrame].mValue.z;

			pose.myQuaternionTest.x = channel->mRotationKeys[currentFrame].mValue.x;
			pose.myQuaternionTest.z = channel->mRotationKeys[currentFrame].mValue.y;
			pose.myQuaternionTest.y = channel->mRotationKeys[currentFrame].mValue.z;
			pose.myQuaternionTest.w = channel->mRotationKeys[currentFrame].mValue.w;

			pose.myRotation.x = channel->mRotationKeys[currentFrame].mValue.x;
			pose.myRotation.y = channel->mRotationKeys[currentFrame].mValue.y;
			pose.myRotation.z = channel->mRotationKeys[currentFrame].mValue.z;

			pose.myScale.x = channel->mScalingKeys[currentFrame].mValue.x;
			pose.myScale.y = channel->mScalingKeys[currentFrame].mValue.y;
			pose.myScale.z = channel->mScalingKeys[currentFrame].mValue.z;

			newFrame.myPose.insert({ channel->mNodeName.C_Str(), pose });
		}

		anim.myKeyFrames.push_back(newFrame);
		currentFrame++;
	}

	// Add to loaded model list
	myLoadedAnimations.insert({ aAnimationFileName, anim });
}

void ModelAssetHandler::LoadTexture(std::string aTextureFileName)
{
	if (myLoadedTextures.find(aTextureFileName) != myLoadedTextures.end())
	{
		LOG_WARNING("Texture has been already loaded.");
		Assert(false);
	}

	TextureData textureData;

	// Init SRV and Sampler
	textureData.mySRV.Init(aTextureFileName);
	textureData.mySampler.Init();

	// Add to loaded texture list
	myLoadedTextures.insert({ aTextureFileName, textureData });
}

ModelData& ModelAssetHandler::GetModelData(std::string aModelFileName)
{
	if (myLoadedModels.find(aModelFileName) == myLoadedModels.end())
	{
		LOG_ERROR("Model not found.");
		Assert(false);
	}
	
	return myLoadedModels.at(aModelFileName);
}

TextureData& ModelAssetHandler::GetTextureData(std::string aTextureFileName)
{
	if (myLoadedTextures.find(aTextureFileName) == myLoadedTextures.end())
	{
		LOG_ERROR("Texture not found.");
		Assert(false);
	}

	return myLoadedTextures.at(aTextureFileName);
}

Animation& ModelAssetHandler::GetAnimation(std::string aAnimationFileName)
{
	if (myLoadedAnimations.find(aAnimationFileName) == myLoadedAnimations.end())
	{
		LOG_ERROR("Animation not found.");
		Assert(false);
	}

	return myLoadedAnimations.at(aAnimationFileName);
}

int ModelAssetHandler::currentBoneIndex = 1;

void ModelAssetHandler::CreateBoneHierarchy(aiNode* aNode, Bone* aBone)
{
	for (int i = 0; i < aNode->mNumChildren; i++)
	{
		aiNode* child = aNode->mChildren[i];

		if (names.find(child->mName.C_Str()) == names.end())
		{
			CreateBoneHierarchy(child, aBone);
			continue;
		}

		Bone bone;
		bone.myName = child->mName.C_Str();
		bone.myId = currentBoneIndex;
		currentBoneIndex++;

		aiMatrix4x4 t = child->mTransformation.Inverse();
		bone.myInverseBindTransform = DirectX::XMMatrixSet(
			t.a1, t.a2, t.a3, t.a4,
			t.b1, t.b2, t.b3, t.b4,
			t.c1, t.c2, t.c3, t.c4,
			t.d1, t.d2, t.d3, t.d4);

		aBone->myChildren.push_back(bone);
		CreateBoneHierarchy(child, &aBone->myChildren[i]);
	}
}

aiNode* ModelAssetHandler::GetPreRotation(aiNode* aNode)
{
	if (std::string(aNode->mParent->mName.C_Str()).find("PreRotation") != std::string::npos)
	{
		return aNode->mParent;
	}

	if (std::string(aNode->mParent->mName.C_Str()).find("Rotation") != std::string::npos)
	{
		return aNode->mParent->mParent;
	}

	return aNode;
}


