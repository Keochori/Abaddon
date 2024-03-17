#pragma once
#include "Graphics/Vertex.h"
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11Buffer;

class VertexBuffer
{
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

	void Init(std::vector<Vertex> aVertexList);
	void Bind();
	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAdressOf() const;

private:
	ComPtr<ID3D11Buffer> myBuffer;
};

