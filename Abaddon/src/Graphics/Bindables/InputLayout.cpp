#include "pch.h"
#include "InputLayout.h"
#include "Graphics/DX11.h"
#include <d3dcompiler.h>

void InputLayout::Init(std::vector<D3D11_INPUT_ELEMENT_DESC> aDescriptionList, std::string aVertexShaderFileName)
{
	// Get Vertex Shader info
	ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob(AddStringsReturnWStr("Shaders/", aVertexShaderFileName).c_str(), &blob);

	// Create 
	HRESULT hr = DX11::ourDevice->CreateInputLayout(std::data(aDescriptionList), (UINT)aDescriptionList.size(),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&myInputLayout);
	DX11::HRASSERT(hr, "Creating Input Layout");
}

void InputLayout::Bind()
{
	DX11::ourContext->IASetInputLayout(myInputLayout.Get());
}

ID3D11InputLayout* InputLayout::Get() const
{
	return myInputLayout.Get();
}

ID3D11InputLayout* const* InputLayout::GetAdressOf() const
{
	return  myInputLayout.GetAddressOf();
}
