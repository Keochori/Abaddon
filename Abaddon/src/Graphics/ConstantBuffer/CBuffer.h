#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "Log.h"

using namespace Microsoft::WRL;

template<typename T>
class CBuffer
{
public:
	CBuffer() = default;
	~CBuffer() = default;

	HRESULT Init(ComPtr<ID3D11Device> aDevice, ComPtr<ID3D11DeviceContext> aContext);
	HRESULT ApplyChanges();

	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAdressOf() const;

	T myData;

private:
	ComPtr<ID3D11Buffer> myBuffer;
	ComPtr<ID3D11Device> myDevice;
	ComPtr<ID3D11DeviceContext> myContext;
};

template<typename T>
inline HRESULT CBuffer<T>::Init(ComPtr<ID3D11Device> aDevice, ComPtr<ID3D11DeviceContext> aContext)
{
	myDevice = aDevice;
	myContext = aContext;

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

	HRESULT hr = myDevice->CreateBuffer(&bufferDesc, &subResData, myBuffer.GetAddressOf());
	
	return hr;
}

template<typename T>
inline HRESULT CBuffer<T>::ApplyChanges()
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = myContext->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

	memcpy(bufferData.pData, &myData, sizeof(myData));

	myContext->Unmap(myBuffer.Get(), 0);

	return hr;
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
