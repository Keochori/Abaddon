#include "pch.h"
#include "VertexBuffer.h"
#include "Graphics/DX11.h"

void VertexBuffer::Init(std::vector<Vertex> aVertexList)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = UINT(sizeof(Vertex) * aVertexList.size());
	bufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData.pSysMem = std::data(aVertexList);

	// Create
	HRESULT hr = DX11::ourDevice->CreateBuffer(&bufferDesc, &subResData, &myBuffer);
	DX11::HRASSERT(hr, "Creating Vertex Buffer");
}

void VertexBuffer::Bind()
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	DX11::ourContext->IASetVertexBuffers(0u, 1u, myBuffer.GetAddressOf(), &stride, &offset);
}

ID3D11Buffer* VertexBuffer::Get() const
{
	return myBuffer.Get();
}

ID3D11Buffer* const* VertexBuffer::GetAdressOf() const
{
	return myBuffer.GetAddressOf();
}