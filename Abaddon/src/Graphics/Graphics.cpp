#include "pch.h"
#include "Graphics.h"
#include <d3dcompiler.h>

#include "Vertex.h"
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"

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
	ComPtr<ID3D11DepthStencilState> depthStencilState;

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	HRESULT hr = myDevice->CreateDepthStencilState(&depthDesc, &depthStencilState);
	HRASSERT(hr, "Creation of Depth Stencil State");

	myContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

	// Depth Stencil Texture
	ComPtr<ID3D11Texture2D> depthStencilTexture;

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

	hr = myDevice->CreateTexture2D(&textureDesc, nullptr, &depthStencilTexture);
	HRASSERT(hr, "Creation of Depth Stencil Texture 2D");

	// Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = myDevice->CreateDepthStencilView(depthStencilTexture.Get(), &depthStencilViewDesc, &myDepthStencilView);
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

void Graphics::CreateAndSetTexture2D(std::wstring aTextureFileName, ComPtr<ID3D11ShaderResourceView>& aSRV)
{
	HRESULT hr;

	// Image
	auto image_data = DirectX::ScratchImage{};
	hr = DirectX::LoadFromWICFile(aTextureFileName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image_data);
	HRASSERT(hr, "Loading Texture image");

	// Texture
	ComPtr<ID3D11Resource> texture;

	hr = DirectX::CreateTexture(myDevice.Get(), image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &texture);
	HRASSERT(hr, "Creating Texture");

	// Shader Resource View "SRV"
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = image_data.GetMetadata().format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = myDevice->CreateShaderResourceView(texture.Get(), &srvDesc, &aSRV);
	HRASSERT(hr, "Creating Shader Resource View");

	myContext->PSSetShaderResources(0, 1, aSRV.GetAddressOf());
	

	// Sampler 
	ComPtr<ID3D11SamplerState> samplerState;
	D3D11_SAMPLER_DESC samplerDesc = {};

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hr = myDevice->CreateSamplerState(&samplerDesc, &samplerState);
	HRASSERT(hr, "Creating sampler");

	myContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
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
	math::vector3<float> positions[] = {
		// Front
		{-1.0f, 1.0f,-1.0f},
		{ 1.0f, 1.0f,-1.0f},
		{-1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},

		// Back
		{-1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f},
		{-1.0f,-1.0f, 1.0f},
		{ 1.0f,-1.0f, 1.0f}
	};

	math::vector2<float> texcoords[] = {
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},
	};

	// Vertex buffer
	CreateAndSetVertexBuffer({
		// Front
		{positions[0], texcoords[0]},
		{positions[1], texcoords[1]},
		{positions[2], texcoords[2]},
		{positions[3], texcoords[3]},

		// Back
		{positions[4], texcoords[1]},
		{positions[5], texcoords[0]},
		{positions[6], texcoords[3]},
		{positions[7], texcoords[2]},

		// Left
		{positions[4], texcoords[0]},
		{positions[0], texcoords[1]},
		{positions[6], texcoords[2]},
		{positions[2], texcoords[3]},

		// Right
		{positions[1], texcoords[0]},
		{positions[5], texcoords[1]},
		{positions[3], texcoords[2]},
		{positions[7], texcoords[3]},

		// Top
		{positions[4], texcoords[0]},
		{positions[5], texcoords[1]},
		{positions[0], texcoords[2]},
		{positions[1], texcoords[3]},

		// Bottom
		{positions[2], texcoords[0]},
		{positions[3], texcoords[1]},
		{positions[6], texcoords[2]},
		{positions[7], texcoords[3]}
		});

	// Index buffer
	CreateAndSetIndexBuffer({
		// Front
		0,1,3, 0,3,2,

		// Back
		5,4,6, 5,6,7,

		// Left
		8,9,11, 8,11,10,

		// Right
		12,13,15, 12,15,14,

		// Top
		16,17,19, 16,19,18,

		// Bottom
		20,21,23, 20,23,22
		});

	// Input Layout
	CreateAndSetInputLayout({
		{"POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		}, "VertexShader_vs.cso");

	// Transform Buffer
	HRASSERT(aCube.myTransformBuffer.Init(myDevice, myContext), "Initializing cube");
	aCube.myTransformBuffer.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationZ(1) *
			DirectX::XMMatrixRotationY(1) *
			DirectX::XMMatrixTranslation(aCube.myPosition.x, aCube.myPosition.y, aCube.myPosition.z) *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.5f, 20.0f)
		);
	HRASSERT(aCube.myTransformBuffer.ApplyChanges(), "Applying changes to cube");
	myContext->VSSetConstantBuffers(0, 1, aCube.myTransformBuffer.GetAdressOf());
}

void Graphics::UpdateCube(Cube& aCube, float aRotation)
{
	aCube.myTransformBuffer.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationZ(aRotation) *
			DirectX::XMMatrixRotationY(aRotation) *
			DirectX::XMMatrixTranslation(aCube.myPosition.x, aCube.myPosition.y, aCube.myPosition.z) *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.5f, 20.0f)
		);

	// Set Transform Buffer
	HRASSERT(aCube.myTransformBuffer.ApplyChanges(), "Applying changes to cube", false);
	myContext->VSSetConstantBuffers(0, 1, aCube.myTransformBuffer.GetAdressOf());

	// Set Texture
	myContext->PSSetShaderResources(0, 1, aCube.mySRV.GetAddressOf());
}

void Graphics::InitGame()
{
	// Shaders
	CreateAndSetVertexShader("VertexShader_vs.cso");
	CreateAndSetPixelShader("PixelShader_ps.cso");

	// Cube
	myCubes.resize(2);
	Create3DCube(myCubes[0]);
	Create3DCube(myCubes[1]);

	CreateAndSetTexture2D(L"brick.jpg", myCubes[0].mySRV);
	CreateAndSetTexture2D(L"rock.png", myCubes[1].mySRV);

	myCubes[0].myPosition = { 2.0f,0.0f,8.0f };
	myCubes[1].myPosition = { -2.0f,0.0f,8.0f };
}

void Graphics::Update(float aRotation)
{
	ClearRenderTargetView();

	UpdateCube(myCubes[0], aRotation);
	DrawIndexed(36);

	UpdateCube(myCubes[1], -aRotation + 20);
	DrawIndexed(36);
}

