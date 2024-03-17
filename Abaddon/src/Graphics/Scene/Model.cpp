#include "pch.h"
#include "Model.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

void Model::LoadModel(std::string aFilePath, std::shared_ptr<Camera> aCamera)
{
	Assimp::Importer importer;
	auto modelData = importer.ReadFile(aFilePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	// Vertex Buffer
	std::vector<Vertex> vertexList;
	for (int mIndex = 0; mIndex < modelData->mNumMeshes; mIndex++)
	{
		auto mesh = modelData->mMeshes[mIndex];

		for (int vIndex = 0; vIndex < mesh->mNumVertices; vIndex++)
		{
			auto vertex = mesh->mVertices[vIndex];
			vertexList.push_back({ vertex.x, vertex.y, vertex.z, 0, 0 });
		}
	}
	myModelData.myVertexBuffer.Init(vertexList);

	// Index
	std::vector<unsigned short> indexList;
	for (int mIndex = 0; mIndex < modelData->mNumMeshes; mIndex++)
	{
		auto mesh = modelData->mMeshes[mIndex];

		for (int fIndex = 0; fIndex < mesh->mNumFaces; fIndex++)
		{
			auto face = mesh->mFaces[fIndex];
			indexList.push_back(face.mIndices[0]);
			indexList.push_back(face.mIndices[1]);
			indexList.push_back(face.mIndices[2]);
		}
	}
	myModelData.myIndexBuffer.Init(indexList);

	// Input Layout
	myModelData.myInputLayout.Init({
		{ "POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		}, "VertexShader_vs.cso");

	// SRV
	myModelData.mySRV.Init(L"rock.png");

	// Sampler
	myModelData.mySampler.Init();

	// Transform buffer
	myModelData.myTransformCBuffer.Init(eBindType::VertexShader);
	myModelData.myTransformCBuffer.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			//DirectX::XMMatrixRotationZ(0) *
			//DirectX::XMMatrixRotationY(0) *
			DirectX::XMMatrixTranslation(0, 0, 2) *
			aCamera->GetMatrix() *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f)
		);
	myModelData.myTransformCBuffer.ApplyChanges();
	myModelData.myTransformCBuffer.Bind();
}

void Model::Update(std::shared_ptr<Camera> aCamera)
{
	// Transform buffer
	myModelData.myTransformCBuffer.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			//DirectX::XMMatrixRotationZ(0) *
			//DirectX::XMMatrixRotationY(0) *
			DirectX::XMMatrixTranslation(0, 0, 2) *
			aCamera->GetMatrix() *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f)
		);
	myModelData.myTransformCBuffer.ApplyChanges();
	myModelData.myTransformCBuffer.Bind();
}

ModelData& Model::GetModelData()
{
	return myModelData;
}
