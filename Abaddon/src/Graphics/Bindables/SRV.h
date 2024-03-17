#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11ShaderResourceView;

class SRV
{
public:
	SRV() = default;
	~SRV() = default;

	void Init(std::string aTextureFileName);
	void Bind();
	ID3D11ShaderResourceView* Get() const;
	ID3D11ShaderResourceView* const* GetAdressOf() const;

private:
	ComPtr<ID3D11ShaderResourceView> mySRV;
};

