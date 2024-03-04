#pragma once
#include <windows.h>
#include <d3d11.h>
#include <wrl.h>

#if defined(DEBUG) || defined(RELEASE)
#include <system_error>
#endif

using namespace Microsoft::WRL;

struct Vertex;
struct TestCBuffer;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Init(HWND& aWindow);
	void Init(HWND& aWindow, float aClearColor[4]);
	void EndFrame();

	void Update(float aRotation);

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
	ComPtr<ID3D11Buffer> CreateAndSetConstantBuffer(TestCBuffer aBufferData);
	void UpdateCBuffer(TestCBuffer aBufferData);

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

	ComPtr<ID3D11Buffer> myCBuffer;

	HWND myWindow;
};


