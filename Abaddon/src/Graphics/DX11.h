#pragma once
#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class DX11
{
public:
	DX11(HWND& aWindowHandle);
	~DX11() = default;

	void Initialize(bool aDebugMode);
	void BeginFrame(float aClearColor[4]);
	void BindRenderTarget();
	void EndFrame();

	static ComPtr<ID3D11Device> ourDevice;
	static ComPtr<IDXGISwapChain> ourSwapChain;
	static ComPtr<ID3D11DeviceContext> ourContext;
	static ComPtr<ID3D11RenderTargetView> ourBackBuffer;
	static ComPtr<ID3D11DepthStencilView> ourDepthBuffer;

	static void HRASSERT(HRESULT aHr, std::string aDescription, bool aPrint = true);

private:
	void CreateRenderTargetView();
	void CreateDepth();
	void SetViewPort();
	void SetPrimitiveTopology();
	void SetVertexShader(std::string aShaderFileName);
	void SetPixelShader(std::string aShaderFileName);

	int GetScreenWidth();
	int GetScreenHeight();

	HWND& myWindowHandle;
};

