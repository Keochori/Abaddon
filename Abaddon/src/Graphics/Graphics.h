#pragma once
#include <windows.h>
#include <d3d11.h>
#include <wrl.h>
#include "ConstantBuffer/CBuffer.h"
#include "ConstantBuffer/CBufferStructs.h"

#include "Math/vectors.h"

#if defined(DEBUG) || defined(RELEASE)
#include <system_error>
#endif

using namespace Microsoft::WRL;

struct Vertex;
struct TestCBuffer;

struct Cube
{
	math::vector3<float> myPosition = { 0,0,10.0f };

	ComPtr<ID3D11ShaderResourceView> mySRV;
	CBuffer<TransformBuffer> myTransformBuffer;
};

class Graphics
{
public:
	Graphics(HWND& aWindow);
	~Graphics();

	void Init();
	void Init(float aClearColor[4]);
	void EndFrame();

	void Update(float aRotation);

private:
	void SetPrimitiveTopology();
	void SetViewPort();

	void InitGame();

	void DrawIndexed(const unsigned int aVertexAmount);

	void CreateDepth();
	void BindRenderTarget();
	void CreateRenderTargetView();
	void ClearRenderTargetView();

	void CreateAndSetVertexBuffer(std::vector<Vertex> aVertexList);
	void CreateAndSetIndexBuffer(std::vector<unsigned short> aIndexList);
	void CreateAndSetVertexShader(std::string aShaderFileName);
	void CreateAndSetPixelShader(std::string aShaderFileName);
	void CreateAndSetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> aDescriptionList, std::string aVertexShaderFileName);
	void CreateAndSetTexture2D(std::wstring aTextureFileName, ComPtr<ID3D11ShaderResourceView>& aSRV);

	void Create3DCube(Cube& aCube);
	void UpdateCube(Cube& aCube, float aRotation);

	int GetWidth();
	int GetHeight();

	void inline HRASSERT(HRESULT aHr, std::string aDescription, bool aPrint = true)
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

	float myClearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	ComPtr<ID3D11Device> myDevice;
	ComPtr<IDXGISwapChain> mySwapChain;
	ComPtr<ID3D11DeviceContext> myContext;
	ComPtr<ID3D11RenderTargetView> myTarget;
	ComPtr<ID3D11DepthStencilView> myDepthStencilView;

	std::vector<Cube> myCubes;

	HWND myWindow;
};


