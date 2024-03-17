#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11Buffer;

class IndexBuffer
{
public:
	IndexBuffer() = default;
	~IndexBuffer() = default;

	void Init(std::vector<unsigned short> aIndexList);
	void Bind();
	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAdressOf() const;

	unsigned int GetIndexAmount() const;

private:
	ComPtr<ID3D11Buffer> myBuffer;
	unsigned int myIndexAmount;
};

