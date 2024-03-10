#include "pch.h"
#include "Graphics.h"
#include <d3dcompiler.h>

#include "Vertex.h"
#include <DirectXMath.h>

Graphics::Graphics(HWND& aWindow) : myWindow (aWindow)
{
}

Graphics::~Graphics()
{
}

void Graphics::Init()
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
	desc.OutputWindow = myWindow;
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

	CreateRenderTargetView();
	CreateDepth();
	BindRenderTarget();

	SetViewPort();
	SetPrimitiveTopology();

	InitGame();
}

void Graphics::Init(float aClearColor[4])
{
	Init();

	for (int i = 0; i < 3; i++)
	{
		myClearColor[i] = aClearColor[i];
	}
}

void Graphics::SetPrimitiveTopology()
{
	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::SetViewPort()
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)GetWidth();
	vp.Height = (float)GetHeight();
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

void Graphics::CreateDepth()
{
	// Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ComPtr<ID3D11DepthStencilState> depthStencilState;
	
	HRESULT hr = myDevice->CreateDepthStencilState(&depthDesc, &depthStencilState);
	HRASSERT(hr, "Creation of Depth Stencil State");

	myContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

	// Depth Stencil Texture
	ComPtr<ID3D11Texture2D> depthStencil;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = GetWidth();
	textureDesc.Height = GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = myDevice->CreateTexture2D(&textureDesc, nullptr, &depthStencil);
	HRASSERT(hr, "Creation of Depth Stencil Texture 2D");

	// Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = myDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, &myDepthStencilView);
	HRASSERT(hr, "Creation of Depth Stencil View");
}

void Graphics::BindRenderTarget()
{
	myContext->OMSetRenderTargets(1, myTarget.GetAddressOf(), myDepthStencilView.Get());
}

void Graphics::ClearRenderTargetView()
{
	myContext->ClearRenderTargetView(myTarget.Get(), myClearColor);
	myContext->ClearDepthStencilView(myDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
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
	bufferDesc.ByteWidth = UINT(sizeof(Vertex) * aVertexList.size());
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
	bufferDesc.ByteWidth = UINT(sizeof(unsigned short) * aIndexList.size());
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
	D3DReadFileToBlob(AddStringsReturnWStr("Shaders/", aShaderFileName).c_str(), &blob);

	HRESULT hr = myDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
	HRASSERT(hr, "Creating and setting Vertex Shader");

	myContext->VSSetShader(vertexShader.Get(), nullptr, 0);
}

void Graphics::CreateAndSetPixelShader(std::string aShaderFileName)
{
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11PixelShader> pixelShader;
	D3DReadFileToBlob(AddStringsReturnWStr("Shaders/", aShaderFileName).c_str(), &blob);

	HRESULT hr = myDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	HRASSERT(hr, "Creating and setting Pixel Shader");

	myContext->PSSetShader(pixelShader.Get(), nullptr, 0);
}

void Graphics::CreateAndSetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> aDescriptionList, std::string aVertexShaderFileName)
{
	// Get Vertex Shader data
	ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob(AddStringsReturnWStr("Shaders/", aVertexShaderFileName).c_str(), &blob);

	// Create 
	ComPtr<ID3D11InputLayout> inputLayout;

	HRESULT hr = myDevice->CreateInputLayout(std::data(aDescriptionList), (UINT)aDescriptionList.size(),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&inputLayout);

	HRASSERT(hr, "Creating and setting Input Layout");

	// Set
	myContext->IASetInputLayout(inputLayout.Get());
}

int Graphics::GetWidth()
{
	RECT rect;
	GetClientRect(myWindow, &rect);

	return rect.right - rect.left;
}

int Graphics::GetHeight()
{
	RECT rect;
	GetClientRect(myWindow, &rect);

	return rect.bottom - rect.top;
}

void Graphics::DrawIndexed(const unsigned int aVertexAmount)
{
	myContext->DrawIndexed(aVertexAmount, 0, 0);
}

void Graphics::Create3DCube(Cube& aCube)
{
	// Vertex buffer
	CreateAndSetVertexBuffer({
		{-1.0f,-1.0f,-1.0f,},
		{ 1.0f,-1.0f,-1.0f,},
		{-1.0f, 1.0f,-1.0f,},
		{ 1.0f, 1.0f,-1.0f,},

		{-1.0f,-1.0f, 1.0f,},
		{ 1.0f,-1.0f, 1.0f,},
		{-1.0f, 1.0f, 1.0f,},
		{ 1.0f, 1.0f, 1.0f,}
		});

	// Index buffer
	CreateAndSetIndexBuffer({
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
		});

	// Input Layout
	CreateAndSetInputLayout({
		{"POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		}, "VertexShader_vs.cso");

	// Transform Buffer
	HRASSERT(aCube.myTransformBuffer.Init(myDevice, myContext), "Initializing cube");
	aCube.myTransformBuffer.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationZ(1) *
			DirectX::XMMatrixRotationY(1) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 10.0f) *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.5f, 20.0f)
		);
	HRASSERT(aCube.myTransformBuffer.ApplyChanges(), "Applying changes to cube");
	myContext->VSSetConstantBuffers(0, 1, aCube.myTransformBuffer.GetAdressOf());

	// Face Colors Buffer
	HRASSERT(aCube.myFaceColorsBuffer.Init(myDevice, myContext), "Initializing cube");
	aCube.myFaceColorsBuffer.myData.myFaceColors[0] = { 1.0f,0.0f,0.0f };
	aCube.myFaceColorsBuffer.myData.myFaceColors[1] = { 0.0f,1.0f,0.0f };
	aCube.myFaceColorsBuffer.myData.myFaceColors[2] = { 0.0f,0.0f,1.0f };
	aCube.myFaceColorsBuffer.myData.myFaceColors[3] = { 0.0f,1.0f,1.0f };
	aCube.myFaceColorsBuffer.myData.myFaceColors[4] = { 1.0f,0.0f,1.0f };
	aCube.myFaceColorsBuffer.myData.myFaceColors[5] = { 1.0f,1.0f,1.0f };
	HRASSERT(aCube.myFaceColorsBuffer.ApplyChanges(), "Applying changes to cube");
	myContext->PSSetConstantBuffers(0, 1, aCube.myFaceColorsBuffer.GetAdressOf());
}

void Graphics::UpdateCube(Cube& aCube, float aRotation)
{
	aCube.myTransformBuffer.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationZ(aRotation) *
			DirectX::XMMatrixRotationY(aRotation) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 10.0f) *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.5f, 20.0f)
		);

	HRASSERT(aCube.myTransformBuffer.ApplyChanges(), "Applying changes to cube", false);
	myContext->VSSetConstantBuffers(0, 1, aCube.myTransformBuffer.GetAdressOf());
}

void Graphics::InitGame()
{
	// Vertex shader
	CreateAndSetVertexShader("VertexShader_vs.cso");
	CreateAndSetPixelShader("PixelShader_ps.cso");

	myCubes.resize(2);
	Create3DCube(myCubes[0]);
	Create3DCube(myCubes[1]);
}

void Graphics::Update(float aRotation)
{
	ClearRenderTargetView();

	UpdateCube(myCubes[0], aRotation);
	DrawIndexed(36);
	UpdateCube(myCubes[1], -aRotation + 10);
	DrawIndexed(36);
}

