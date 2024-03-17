#include "pch.h"
#include "IndexBuffer.h"
#include "Graphics/DX11.h"

void IndexBuffer::Init(std::vector<unsigned short> aIndexList)
{
	myIndexAmount = aIndexList.size();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = UINT(sizeof(unsigned short) * aIndexList.size());
	bufferDesc.StructureByteStride = sizeof(unsigned short);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA subResData;
	subResData.pSysMem = std::data(aIndexList);

	// Create
	HRESULT hr = DX11::ourDevice->CreateBuffer(&bufferDesc, &subResData, &myBuffer);
	DX11::HRASSERT(hr, "Creating Index Buffer");
}

void IndexBuffer::Bind()
{
	DX11::ourContext->IASetIndexBuffer(myBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

ID3D11Buffer* IndexBuffer::Get() const
{
	return myBuffer.Get();
}

ID3D11Buffer* const* IndexBuffer::GetAdressOf() const
{
	return myBuffer.GetAddressOf();
}

unsigned int IndexBuffer::GetIndexAmount() const
{
	return myIndexAmount;
}
