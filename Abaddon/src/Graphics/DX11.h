#pragma once
#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class DX11
{
public:
	DX11(HWND& aHWND);
	~DX11() = default;

	void Initialize(bool aDebugMode);
	void BeginFrame(float aClearColor[4]);
	void EndFrame();

	static void BindRenderTarget();
	static void BindRenderTargetTexture();

	static void ToggleWireframeRS(bool aOn);

	static void HRASSERT(HRESULT aHr, std::string aDescription, bool aPrint = true);

	static ComPtr<ID3D11Device> ourDevice;
	static ComPtr<ID3D11DeviceContext> ourContext;
	static ComPtr<IDXGISwapChain> ourSwapChain;
	static ComPtr<ID3D11RenderTargetView> ourBackBuffer;
	static ComPtr<ID3D11DepthStencilView> ourDepthBuffer;

	// Render Target View bound to a texture, used by ImGui to render Scene Tab.
	static ComPtr<ID3D11ShaderResourceView> ourTextureSRV;
	static ComPtr<ID3D11RenderTargetView> ourTextureBuffer;
	static ComPtr<ID3D11Texture2D> ourTexture;
	static ComPtr<ID3D11RasterizerState> ourWireframeRS;

private:
	void CreateRenderTargetView();
	void CreateSceneTextureResources();
	void CreateDepth();
	void CreateWireframeRasterizerState();
	void SetViewPort();
	void SetPrimitiveTopology();
	void SetVertexShader(std::string aShaderFileName);
	void SetPixelShader(std::string aShaderFileName);

	int GetScreenWidth();
	int GetScreenHeight();

	HWND& myHWND;
};

