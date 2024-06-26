#include "pch.h"
#include "ModelAssetHandler.h"
#include <DirectXMath.h>
#include <algorithm>

std::unordered_map<std::string, ModelData> ModelAssetHandler::myLoadedModels;
std::unordered_map<std::string, TextureData> ModelAssetHandler::myLoadedTextures;
std::unordered_map<std::string, Animation> ModelAssetHandler::myLoadedAnimations;
const aiScene* ModelAssetHandler::myData;
std::string ModelAssetHandler::myModelName;
Assimp::Importer ModelAssetHandler::importer;

Animation ModelAssetHandler::myCurrentAnim;
Skeleton* ModelAssetHandler::mySkeleton;
int ModelAssetHandler::myCurrentFrame = 0;
float ModelAssetHandler::myTimer = 0;
bool ModelAssetHandler::inited = false;

void ModelAssetHandler::LoadModel(std::string aModelFileName)
{
	// Check if it's already loaded
	if (myLoadedModels.find(aModelFileName) != myLoadedModels.end())
	{
		LOG_WARNING("Model has been already loaded.");
		Assert(false);
		return;
	}

	// Load file
	auto data = importer.ReadFile("Models/" + aModelFileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights);
	myData = data;
	if (!data)
	{
		LOG_ERROR("Failed to load model '" + aModelFileName + "'");
		Assert(false);
	}

	// Vertex Buffer & Index Buffer list & Skeleton
	ModelData modelData;
	std::vector<Vertex> vertexList;
	std::vector<unsigned short> indexList;
	for (int mIndex = 0; mIndex < data->mNumMeshes; mIndex++)
	{
		auto mesh = data->mMeshes[mIndex];
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
			modelData.myHasSkeleton = true;

			Skeleton& skeleton = modelData.mySkeleton;
			skeleton.myName = aModelFileName + ".skeleton";
			skeleton.myBoneAmount = mesh->mNumBones;

			for (int i = 0; i < skeleton.myBoneAmount; i++)
			{
				aiBone* boneData = mesh->mBones[i];

				Bone bone;
				bone.myId = i;
				bone.myName = boneData->mName.C_Str();
				bone.myFinalTransformation = DirectX::XMMatrixIdentity();
				aiMatrix4x4 t = boneData->mOffsetMatrix;
				bone.myOffsetMatrix = DirectX::XMMatrixSet(
					t.a1, t.a2, t.a3, t.a4,
					t.b1, t.b2, t.b3, t.b4,
					t.c1, t.c2, t.c3, t.c4,
					t.d1, t.d2, t.d3, t.d4);

				skeleton.myBoneNameToIndexMap.insert({ bone.myName, bone.myId });
				skeleton.myBones.push_back(bone);
			}

			ReadBoneHierarchy(data->mRootNode, DirectX::XMMatrixIdentity(), skeleton);

			// Vertex bone ids & weight data
			for (int bone_i = 0; bone_i < mesh->mNumBones; bone_i++)
			{
				auto bone = mesh->mBones[bone_i];

				for (int weight_i = 0; weight_i < bone->mNumWeights; weight_i++)
				{
					auto weight = bone->mWeights[weight_i];

					Vertex& vertex = vertexList[weight.mVertexId];
					for (int i = 0; i < MAX_BONES; i++)
					{
						unsigned int currentBoneValue = vertex.myBoneIDs[i];

						if (currentBoneValue == -1)
						{
							vertex.myBoneIDs[i] = bone_i;
							vertex.myBoneWeights[i] = weight.mWeight;
							break;
						}
					}
				}
			}
		}
		else
		{
			modelData.myHasSkeleton = false;
		}
	}

	// Create Input Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "BONEIDS" , 0,DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BONEWEIGHTS", 0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

	// Init
	modelData.myVertexBuffer.Init(vertexList);
	modelData.myIndexBuffer.Init(indexList);
	modelData.myInputLayout.Init(inputLayout, "VertexShader_vs.cso");

	// Add to loaded model list
	myLoadedModels.insert({ aModelFileName, modelData });
}

void ModelAssetHandler::ReadBoneHierarchy(aiNode* aNode, DirectX::XMMATRIX aParentTransform, Skeleton& aSkeleton)
{
	std::string nodeName = aNode->mName.C_Str();

	aiMatrix4x4 t = aNode->mTransformation;
	DirectX::XMMATRIX nodeTransform = DirectX::XMMatrixSet(
			t.a1, t.a2, t.a3, t.a4,
			t.b1, t.b2, t.b3, t.b4,
			t.c1, t.c2, t.c3, t.c4,
			t.d1, t.d2, t.d3, t.d4);

	DirectX::XMMATRIX globalTransformation = aParentTransform * nodeTransform;

	std::unordered_map<std::string, int>& boneNameToIndexMap = aSkeleton.myBoneNameToIndexMap;
	if (boneNameToIndexMap.find(nodeName) != boneNameToIndexMap.end())
	{
		int boneIndex = boneNameToIndexMap.at(nodeName);

		Bone& bone = aSkeleton.myBones[boneIndex];
		DirectX::XMMATRIX test = globalTransformation * bone.myOffsetMatrix;
		bone.myFinalTransformation = globalTransformation * bone.myOffsetMatrix;
	}

	for (int i = 0; i < aNode->mNumChildren; i++)
	{
		ReadBoneHierarchy(aNode->mChildren[i], globalTransformation, aSkeleton);
	}
}

void ModelAssetHandler::SetAnim(Animation aAnimation, Skeleton* aSkeleton, std::string aModelName)
{
	myModelName = aModelName;
	myCurrentAnim = aAnimation;
	mySkeleton = aSkeleton;
	inited = true;
}

void ModelAssetHandler::Update()
{
	myTimer += Timer::GetInstance().GetDeltaTime() * 10;

	UpdateTransform(myData->mRootNode, DirectX::XMMatrixIdentity(), *mySkeleton);

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

	//LOG(std::to_string(myCurrentFrame));
}

void ModelAssetHandler::UpdateTransform(aiNode* aNode, DirectX::XMMATRIX aParentTransform, Skeleton& aSkeleton)
{
	std::string nodeName = aNode->mName.C_Str();

	std::unordered_map<std::string, int>& boneNameToIndexMap = aSkeleton.myBoneNameToIndexMap;
	aiMatrix4x4 t = aNode->mTransformation;
	DirectX::XMMATRIX nodeTransform = DirectX::XMMatrixSet(
		t.a1, t.a2, t.a3, t.a4,
		t.b1, t.b2, t.b3, t.b4,
		t.c1, t.c2, t.c3, t.c4,
		t.d1, t.d2, t.d3, t.d4);

	if (myCurrentAnim.myKeyFrames[myCurrentFrame].myPose.find(nodeName) != myCurrentAnim.myKeyFrames[myCurrentFrame].myPose.end())
	{
		math::vector3<float> position = myCurrentAnim.myKeyFrames[myCurrentFrame].myPose.at(nodeName).myPosition;
		math::vector4<float> quaternion = myCurrentAnim.myKeyFrames[myCurrentFrame].myPose.at(nodeName).myQuaternion;
		math::vector3<float> scale = myCurrentAnim.myKeyFrames[myCurrentFrame].myPose.at(nodeName).myScale;
		DirectX::XMMATRIX result =
			DirectX::XMMatrixTranspose(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w)) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z)
			);
		nodeTransform = result;

		std::string log = nodeName + "[" + std::to_string(quaternion.x) + "]" + " [" + std::to_string(quaternion.y) + "] [" + std::to_string(quaternion.z) + "]";
		LOG(log);
	}

	DirectX::XMMATRIX globalTransformation = aParentTransform * nodeTransform;

	if (boneNameToIndexMap.find(nodeName) != boneNameToIndexMap.end())
	{
		int boneIndex = boneNameToIndexMap.at(nodeName);

		Bone& bone = aSkeleton.myBones[boneIndex];
		DirectX::XMMATRIX test = globalTransformation * bone.myOffsetMatrix;
		bone.myFinalTransformation = DirectX::XMMatrixTranspose(globalTransformation * bone.myOffsetMatrix);
	}

	for (int i = 0; i < aNode->mNumChildren; i++)
	{
		UpdateTransform(aNode->mChildren[i], globalTransformation, aSkeleton);
	}
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

			pose.myQuaternion.x = channel->mRotationKeys[currentFrame].mValue.x;
			pose.myQuaternion.y = channel->mRotationKeys[currentFrame].mValue.y;
			pose.myQuaternion.z = channel->mRotationKeys[currentFrame].mValue.z;
			pose.myQuaternion.w = channel->mRotationKeys[currentFrame].mValue.w;

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

Animation& ModelAssetHandler::GetAnimation(std::string aAnimationFileName)
{
	if (myLoadedAnimations.find(aAnimationFileName) == myLoadedAnimations.end())
	{
		LOG_ERROR("Animation not found.");
		Assert(false);
	}

	return myLoadedAnimations.at(aAnimationFileName);
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

