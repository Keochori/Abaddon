#include "pch.h"
#include "DX11.h"
#include <d3dcompiler.h>

ComPtr<ID3D11Device> DX11::ourDevice;
ComPtr<IDXGISwapChain> DX11::ourSwapChain;
ComPtr<ID3D11DeviceContext> DX11::ourContext;

ComPtr<ID3D11RenderTargetView> DX11::ourBackBuffer;
ComPtr<ID3D11DepthStencilView> DX11::ourDepthBuffer;

DX11::DX11(HWND& aHWND) : myHWND(aHWND)
{
}

void DX11::Initialize(bool aDebugMode)
{
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = myHWND;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		aDebugMode ? D3D11_CREATE_DEVICE_DEBUG : 0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		&ourSwapChain,
		&ourDevice,
		nullptr,
		&ourContext);

	HRASSERT(hr, "Initializing DX11 Framework");

	CreateRenderTargetView();
	CreateDepth();
	BindRenderTarget();
	SetViewPort();
	SetPrimitiveTopology();
	SetVertexShader("VertexShader_vs.cso");
	SetPixelShader("PixelShader_ps.cso");
}

void DX11::BeginFrame(float aClearColor[4])
{
	ourContext->ClearRenderTargetView(ourBackBuffer.Get(), aClearColor);
	ourContext->ClearDepthStencilView(ourDepthBuffer.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DX11::EndFrame()
{
	ourSwapChain->Present(1, 0);
}

void DX11::HRASSERT(HRESULT aHr, std::string aDescription, bool aPrint)
{
	if (FAILED(aHr))
	{
		LOG_ERROR(aDescription + " failed.");

		std::string message = "";
#if defined(DEBUG) || defined(RELEASE)
		message = std::system_category().message(aHr);
#endif
		LOG_ERROR("ERROR: " + message);
		Assert(false);
	}
	else
	{
		if (aPrint)
			LOG_SUCCESS(aDescription + " succeeded.");
	}
}

void DX11::CreateRenderTargetView()
{
	ComPtr<ID3D11Resource> backBuffer = nullptr;
	ourSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
	HRESULT hr = ourDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &ourBackBuffer);

	HRASSERT(hr, "Creation of Render Target View");
}

void DX11::CreateDepth()
{
	// Depth Stencil State
	ComPtr<ID3D11DepthStencilState> depthStencilState;

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	HRESULT hr = ourDevice->CreateDepthStencilState(&depthDesc, &depthStencilState);
	HRASSERT(hr, "Creation of Depth Stencil State");

	ourContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

	// Depth Stencil Texture
	ComPtr<ID3D11Texture2D> depthStencilTexture;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = GetScreenWidth();
	textureDesc.Height = GetScreenHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = ourDevice->CreateTexture2D(&textureDesc, nullptr, &depthStencilTexture);
	HRASSERT(hr, "Creation of Depth Stencil Texture 2D");

	// Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = ourDevice->CreateDepthStencilView(depthStencilTexture.Get(), &depthStencilViewDesc, &ourDepthBuffer);
	HRASSERT(hr, "Creation of Depth Stencil View");
}

void DX11::BindRenderTarget()
{
	ourContext->OMSetRenderTargets(1, ourBackBuffer.GetAddressOf(), ourDepthBuffer.Get());
}

void DX11::SetViewPort()
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)GetScreenWidth();
	vp.Height = (float)GetScreenHeight();
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	ourContext->RSSetViewports(1u, &vp);
}

void DX11::SetPrimitiveTopology()
{
	ourContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DX11::SetVertexShader(std::string aShaderFileName)
{
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11VertexShader> vertexShader;
	D3DReadFileToBlob(AddStringsReturnWStr("Shaders/", aShaderFileName).c_str(), &blob);

	HRESULT hr = ourDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
	HRASSERT(hr, "Creating and setting Vertex Shader");

	ourContext->VSSetShader(vertexShader.Get(), nullptr, 0);
}

void DX11::SetPixelShader(std::string aShaderFileName)
{
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11PixelShader> pixelShader;
	D3DReadFileToBlob(AddStringsReturnWStr("Shaders/", aShaderFileName).c_str(), &blob);

	HRESULT hr = ourDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	HRASSERT(hr, "Creating and setting Pixel Shader");

	ourContext->PSSetShader(pixelShader.Get(), nullptr, 0);
}

int DX11::GetScreenWidth()
{
	RECT rect;
	GetClientRect(myHWND, &rect);

	return rect.right - rect.left;
}

int DX11::GetScreenHeight()
{
	RECT rect;
	GetClientRect(myHWND, &rect);

	return rect.bottom - rect.top;
}
