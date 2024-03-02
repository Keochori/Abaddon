#include "pch.h"
#include "Graphics.h"
#include <d3dcompiler.h>

#include "Vertex.h"


Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::Init(HWND aWindow)
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
	desc.OutputWindow = aWindow;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		&mySwapChain,
		&myDevice,
		nullptr,
		&myContext);

	HRASSERT(hr, "Creation of Device and Swapchain");

	// Not mandatory
	CreateRenderTargetView();
	BindRenderTarget();
	SetPrimitiveTopology();
	SetViewPort();

	InitGame();
}

void Graphics::Init(HWND aWindow, float aClearColor[4])
{
	Init(aWindow);

	for (int i = 0; i < 3; i++)
	{
		myClearColor[i] = aClearColor[i];
	}
}

void Graphics::BindRenderTarget()
{
	myContext->OMSetRenderTargets(1u, myTarget.GetAddressOf(), nullptr);
}

void Graphics::SetPrimitiveTopology()
{
	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::SetViewPort()
{
	D3D11_VIEWPORT vp;
	vp.Width = 1424;
	vp.Height = 728;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	myContext->RSSetViewports(1u, &vp);
}

void Graphics::EndFrame()
{
	mySwapChain->Present(1, 0);
}

void Graphics::CreateRenderTargetView()
{
	ComPtr<ID3D11Resource> backBuffer = nullptr;
	mySwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
	HRESULT hr = myDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &myTarget);

	HRASSERT(hr, "Creation of Render Target View");
}

void Graphics::ClearRenderTargetView()
{
	myContext->ClearRenderTargetView(myTarget.Get(), myClearColor);
}

void Graphics::CreateAndSetVertexBuffer(std::vector<Vertex> aVertexList)
{
	// Create
	ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(Vertex) * aVertexList.size();
	bufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData.pSysMem = std::data(aVertexList);
	HRESULT hr = myDevice->CreateBuffer(&bufferDesc, &subResData, &vertexBuffer);

	HRASSERT(hr, "Creating and setting Vertex Buffer");

	// Set
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	myContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
}

void Graphics::CreateAndSetIndexBuffer(std::vector<unsigned short> aIndexList)
{
	ComPtr<ID3D11Buffer> indexBuffer;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned short) * aIndexList.size();
	bufferDesc.StructureByteStride = sizeof(unsigned short);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA subResData;
	subResData.pSysMem = std::data(aIndexList);

	// Create
	HRESULT hr = myDevice->CreateBuffer(&bufferDesc, &subResData, &indexBuffer);
	HRASSERT(hr, "Creating and setting Index Buffer");

	// Set
	myContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

void Graphics::CreateAndSetVertexShader(std::string aShaderFileName)
{
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11VertexShader> vertexShader;
	D3DReadFileToBlob(std::wstring(aShaderFileName.begin(), aShaderFileName.end()).c_str(), &blob);

	HRESULT hr = myDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
	HRASSERT(hr, "Creating and setting Vertex Shader");

	myContext->VSSetShader(vertexShader.Get(), nullptr, 0);
}

void Graphics::CreateAndSetPixelShader(std::string aShaderFileName)
{
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11PixelShader> pixelShader;
	D3DReadFileToBlob(std::wstring(aShaderFileName.begin(), aShaderFileName.end()).c_str(), &blob);

	HRESULT hr = myDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	HRASSERT(hr, "Creating and setting Pixel Shader");

	myContext->PSSetShader(pixelShader.Get(), nullptr, 0);
}

void Graphics::CreateAndSetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> aDescriptionList, std::string aVertexShaderFileName)
{
	// Get Vertex Shader data
	ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob(std::wstring(aVertexShaderFileName.begin(), aVertexShaderFileName.end()).c_str(), &blob);

	// Create 
	ComPtr<ID3D11InputLayout> inputLayout;

	myDevice->CreateInputLayout(std::data(aDescriptionList), aDescriptionList.size(),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&inputLayout);

	// Set
	myContext->IASetInputLayout(inputLayout.Get());
}

void Graphics::DrawIndexed(const unsigned int aVertexAmount)
{
	myContext->DrawIndexed(aVertexAmount, 0, 0);
}

void Graphics::InitGame()
{
	// Vertex buffer
	CreateAndSetVertexBuffer({
		{-0.5f, 0.5f, 0.5f,  1,0,0},
		{ 0.5f, 0.5f, 0.5f,  1,0,1},
		{-0.5f,-0.5f, 0.5f,  1,1,1},
		{ 0.5f,-0.5f, 0.5f,  0,1,1},

		{-0.5f, 0.5f, 1.5f,  0,0,1},
		{ 0.5f, 0.5f, 1.5f,  0,0,0},
		{-0.5f,-0.5f, 1.5f,  0,1,1},
		{ 0.5f,-0.5f, 1.5f,  1,1,1}
		});

	// Index buffer
	CreateAndSetIndexBuffer({
		0,1,3, 0,3,2,
		6,7,5, 6,5,4,
		0,2,6, 0,6,4,
		1,7,3, 1,5,7,
		0,4,5, 0,5,1,
		2,3,7, 2,7,6
		});

	// Vertex shader
	CreateAndSetVertexShader("VertexShader_vs.cso");
	CreateAndSetPixelShader("PixelShader_ps.cso");

	// Input Layout
	CreateAndSetInputLayout({
		{"POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
		}, "VertexShader_vs.cso");
}

void Graphics::DrawStuff()
{
	DrawIndexed(36);
}

void Graphics::Update()
{
	ClearRenderTargetView();

	DrawStuff();
}

