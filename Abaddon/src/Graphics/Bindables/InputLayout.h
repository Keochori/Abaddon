#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

class InputLayout
{
public:
	InputLayout() = default;
	~InputLayout() = default;

	void Init(std::vector<D3D11_INPUT_ELEMENT_DESC> aDescriptionList, std::string aVertexShaderFileName);
	void Bind();
	ID3D11InputLayout* Get() const;
	ID3D11InputLayout* const* GetAdressOf() const;

private:
	ComPtr<ID3D11InputLayout> myInputLayout;
};

