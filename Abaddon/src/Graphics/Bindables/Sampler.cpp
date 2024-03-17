#include "pch.h"
#include "Sampler.h"
#include "Graphics/DX11.h"

void Sampler::Init()
{
	D3D11_SAMPLER_DESC samplerDesc = {};

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	// Create
	HRESULT hr = DX11::ourDevice->CreateSamplerState(&samplerDesc, &mySamplerState);
	DX11::HRASSERT(hr, "Creating Sampler");
}

void Sampler::Bind()
{
	DX11::ourContext->PSSetSamplers(0, 1, mySamplerState.GetAddressOf());
}

ID3D11SamplerState* Sampler::Get() const
{
	return mySamplerState.Get();
}

ID3D11SamplerState* const* Sampler::GetAdressOf() const
{
	return mySamplerState.GetAddressOf();
}
