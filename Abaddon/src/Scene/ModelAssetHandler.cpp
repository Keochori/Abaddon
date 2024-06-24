#include "pch.h"
#include "ModelAssetHandler.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include <DirectXMath.h>
#include <algorithm>

std::unordered_map<std::string, ModelData> ModelAssetHandler::myLoadedModels;
std::unordered_map<std::string, TextureData> ModelAssetHandler::myLoadedTextures;

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
	Assimp::Importer importer;
	auto data = importer.ReadFile("Models/" + aModelFileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights);
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
			Skeleton skeleton;
			skeleton.myName = aModelFileName + ".skeleton";
			skeleton.myBoneAmount = mesh->mNumBones;

			skeleton.myRootBone.myId = 0;
			skeleton.myRootBone.myName = mesh->mBones[0]->mNode->mParent->mName.C_Str();
			skeleton.myRootBone.myInverseBindTransform = DirectX::XMMatrixIdentity();
			skeleton.myRootBone.myAnimatedTransform = DirectX::XMMatrixIdentity();
			skeleton.myRootBone.myResultTranform = DirectX::XMMatrixIdentity();

			std::vector<std::string> validNames;
			for (int i = 0; i < mesh->mNumBones; i++)
			{
				validNames.push_back(mesh->mBones[i]->mName.C_Str());
			}

			CreateBoneHierarchy(mesh->mBones[0]->mNode->mParent, skeleton.myRootBone, validNames);
			modelData.mySkeleton = skeleton;

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

						if (currentBoneValue == 0)
						{
							vertex.myBoneIDs[i] = bone_i + 1;
							vertex.myBoneWeights[i] = weight.mWeight;
							break;
						}
					}
				}
			}
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
	modelData.myVertexBuffer.Init(vertexList);
	modelData.myIndexBuffer.Init(indexList);
	modelData.myInputLayout.Init(inputLayout, "VertexShader_vs.cso");

	// Add to loaded model list
	myLoadedModels.insert({ aModelFileName, modelData });
}

int ModelAssetHandler::currentBoneIndex = 1;

void ModelAssetHandler::CreateBoneHierarchy(aiNode* aNode, Bone& aBone, std::vector<std::string> aValidNames)
{
	for (int i = 0; i < aNode->mNumChildren; i++)
	{
		aiNode* child = aNode->mChildren[i];

		if (std::find(aValidNames.begin(), aValidNames.end(), child->mName.C_Str()) != aValidNames.end())
		{
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

			aBone.myChildren.push_back(bone);
			CreateBoneHierarchy(child, aBone.myChildren[i], aValidNames);

			continue;
		}

		CreateBoneHierarchy(child, aBone, aValidNames);
	}
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
