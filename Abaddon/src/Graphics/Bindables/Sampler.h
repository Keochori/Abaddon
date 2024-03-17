#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11SamplerState;

class Sampler
{
public:
	Sampler() = default;
	~Sampler() = default;

	void Init();
	void Bind();
	ID3D11SamplerState* Get() const;
	ID3D11SamplerState* const* GetAdressOf() const;

private:
	ComPtr<ID3D11SamplerState> mySamplerState;
};

