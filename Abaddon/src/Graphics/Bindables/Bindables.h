#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "CBuffer.hpp"
#include "SRV.h"
#include "Sampler.h"

struct ModelData
{
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;
};

struct TextureData
{
	SRV mySRV;
	Sampler mySampler;
};