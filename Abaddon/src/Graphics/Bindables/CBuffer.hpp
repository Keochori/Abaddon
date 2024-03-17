#pragma once
#include "Graphics/DX11.h"
#include "Graphics/CBufferStructs.h"

using namespace Microsoft::WRL;

enum eBindType
{
	VertexShader,
	PixelShader,
	VsAndPs
};

template<typename T>
class CBuffer
{
public:
	CBuffer() = default;
	~CBuffer() = default;

	void Init(eBindType aBindType);
	void Bind();
	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAdressOf() const;

	void ApplyChanges();
	void SetBindType(eBindType aBindType);
	eBindType GetBindType();

	T myData;

private:
	ComPtr<ID3D11Buffer> myBuffer;
	eBindType myBindType = VsAndPs;
};

template<typename T>
inline void CBuffer<T>::Init(eBindType aBindType)
{
	myBindType = aBindType;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(myData);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResData;
	subResData.pSysMem = &myData;
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;

	// Create
	HRESULT hr = DX11::ourDevice->CreateBuffer(&bufferDesc, &subResData, myBuffer.GetAddressOf());
	DX11::HRASSERT(hr, "Creating CBuffer");
}

template<typename T>
inline void CBuffer<T>::Bind()
{
	switch (myBindType)
	{
		case VertexShader:
			DX11::ourContext->VSSetConstantBuffers(0, 1, myBuffer.GetAddressOf());
			break;

		case PixelShader:
			DX11::ourContext->PSSetConstantBuffers(0, 1, myBuffer.GetAddressOf());
			break;

		case VsAndPs:
			DX11::ourContext->VSSetConstantBuffers(0, 1, myBuffer.GetAddressOf());
			DX11::ourContext->PSSetConstantBuffers(0, 1, myBuffer.GetAddressOf());
			break;

		default:
			Assert(false);
			LOG_ERROR("Can't bind CBuffer, invalid Bind Type.");
			break;
	}
}

template<typename T>
inline void CBuffer<T>::ApplyChanges()
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = DX11::ourContext->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	DX11::HRASSERT(hr, "Applying changes to CBuffer", false);

	memcpy(bufferData.pData, &myData, sizeof(myData));

	DX11::ourContext->Unmap(myBuffer.Get(), 0);
}

template<typename T>
inline void CBuffer<T>::SetBindType(eBindType aBindType)
{
	myBindType = aBindType;
}

template<typename T>
inline eBindType CBuffer<T>::GetBindType()
{
	return myBindType;
}

template<typename T>
inline ID3D11Buffer* CBuffer<T>::Get() const
{
	return myBuffer.Get();
}

template<typename T>
inline ID3D11Buffer* const* CBuffer<T>::GetAdressOf() const
{
	return myBuffer.GetAddressOf();
}
