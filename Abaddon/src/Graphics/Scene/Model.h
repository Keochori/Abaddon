#pragma once
#include "Graphics/Vertex.h"
#include "Graphics/Bindables/Bindables.h"

#include "Camera.h"

struct ModelData
{
	// Mesh
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;

	// Texture
	SRV mySRV;
	Sampler mySampler;

	// CBuffers
	CBuffer<TransformBuffer> myTransformCBuffer;
};

class Model
{
public:
	Model() = default;
	~Model() = default;

	void LoadModel(std::string aFilePath, std::shared_ptr<Camera> aCamera);
	void Update(std::shared_ptr<Camera> aCamera);
	ModelData& GetModelData();

private:
	ModelData myModelData;
};

