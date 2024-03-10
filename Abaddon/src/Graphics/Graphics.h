#pragma once
#include <windows.h>
#include <d3d11.h>
#include <wrl.h>
#include "ConstantBuffer/CBuffer.h"
#include "ConstantBuffer/CBufferStructs.h"

#if defined(DEBUG) || defined(RELEASE)
#include <system_error>
#endif

using namespace Microsoft::WRL;

struct Vertex;
struct TestCBuffer;

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
	void BindRenderTarget();
	void SetPrimitiveTopology();
	void SetViewPort();

	void InitGame();

	void DrawIndexed(const unsigned int aVertexAmount);

	void CreateRenderTargetView();
	void CreateDepth();
	void ClearRenderTargetView();

	void CreateAndSetVertexBuffer(std::vector<Vertex> aVertexList);
	void CreateAndSetIndexBuffer(std::vector<unsigned short> aIndexList);
	void CreateAndSetVertexShader(std::string aShaderFileName);
	void CreateAndSetPixelShader(std::string aShaderFileName);
	void CreateAndSetInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> aDescriptionList, std::string aVertexShaderFileName);

	void Create3DCube(int index);

	int GetWidth();
	int GetHeight();

	void inline HRASSERT(HRESULT aHr, std::string aDescription)
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
			LOG_SUCCESS(aDescription + " succeeded.");
		}
	}

	float myClearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	ComPtr<ID3D11Device> myDevice = nullptr;
	ComPtr<IDXGISwapChain> mySwapChain = nullptr;
	ComPtr<ID3D11DeviceContext> myContext = nullptr;
	ComPtr<ID3D11RenderTargetView> myTarget = nullptr;
	ComPtr<ID3D11DepthStencilView> myDepthStencilView = nullptr;

	CBuffer<TransformBuffer> myTransformBuffer;
	CBuffer<FaceColorsBuffer> myFaceColorsBuffer;

	CBuffer<TransformBuffer> myTransformBuffer2;
	CBuffer<FaceColorsBuffer> myFaceColorsBuffer2;

	HWND myWindow;
};


