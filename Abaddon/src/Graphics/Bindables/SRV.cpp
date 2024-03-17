#include "pch.h"
#include "SRV.h"
#include "Graphics/DX11.h"
#include "DirectXTex/DirectXTex.h"

void SRV::Init(std::string aTextureFileName)
{
	// Image
	auto image_data = DirectX::ScratchImage{};
	HRESULT hr = DirectX::LoadFromWICFile(AddStringsReturnWStr("Textures/", aTextureFileName).c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image_data);
	DX11::HRASSERT(hr, "Loading Texture Image");

	// Texture
	ComPtr<ID3D11Resource> texture;

	hr = DirectX::CreateTexture(DX11::ourDevice.Get(), image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &texture);
	DX11::HRASSERT(hr, "Creating Texture");

	// Creating SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = image_data.GetMetadata().format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = DX11::ourDevice->CreateShaderResourceView(texture.Get(), &srvDesc, &mySRV);
	DX11::HRASSERT(hr, "Creating Shader Resource View");
}

void SRV::Bind()
{
	DX11::ourContext->PSSetShaderResources(0, 1, mySRV.GetAddressOf());
}

ID3D11ShaderResourceView* SRV::Get() const
{
	return mySRV.Get();
}

ID3D11ShaderResourceView* const* SRV::GetAdressOf() const
{
	return mySRV.GetAddressOf();
}
