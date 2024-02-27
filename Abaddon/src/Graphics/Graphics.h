#pragma once
#include <windows.h>
#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

struct Vertex;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Init(HWND aWindow);
	void Init(HWND aWindow, float aClearColor[4]);
	void EndFrame();

	void Update();

private:
	void BindRenderTarget();
	void SetPrimitiveTopology();
	void SetViewPort();

	void InitGame();
	void DrawStuff();

	void DrawIndexed(const unsigned int aVertexAmount);

	void CreateRenderTargetView();
	void ClearRenderTargetView();

	void CreateAndSetVertexBuffer(std::vector<Vertex> aVertexList);
	void CreateAndSetIndexBuffer(std::vector<unsigned short> aIndexList);
	void CreateAndSetVertexShader(std::string aShaderFileName);
	void CreateAndSetPixelShader(std::string aShaderFileName);
	void CreateAndSetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> aDescriptionList, std::string aVertexShaderFileName);

	void inline HRASSERT(HRESULT aHr, std::string aDescription)
	{
		if (FAILED(aHr))
		{
			LOG_ERROR(aDescription + " failed.");
			LOG_ERROR("ERROR: " + aHr);
			assert(false);
		}
		else
		{
			LOG_SUCCESS(aDescription + " succeeded.");
		}
	}

	float myClearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	ComPtr<ID3D11Device> myDevice = nullptr;
	ComPtr<IDXGISwapChain> mySwapChain = nullptr;
	ComPtr<ID3D11DeviceContext> myContext = nullptr;
	ComPtr<ID3D11RenderTargetView> myTarget = nullptr;
};


