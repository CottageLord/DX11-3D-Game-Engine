#include "SYS_CLASS_Graphics.h"


Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;			// default referesh rate
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // default scale
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;		// Sampling mode
	sd.SampleDesc.Quality = 0;		// Anti aliasing
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // use bit-block transfer model
	sd.Flags = 0;

	// create device and front/back buffers, and swap chain and rendering context
	D3D11CreateDeviceAndSwapChain(
		nullptr,					// pAdapter default
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,					// handle to the software driver
		0,							// flag represents devide functionality layers
		nullptr,					// ptr feature levels
		0,							// feature levels
		D3D11_SDK_VERSION,			
		&sd,						// ptr to descriptive struct
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);
	// gain access to texture subresource in swap chain (back buffer)
	ID3D11Resource* pBackBuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
				//   |_ index of buffer, 0 = back buffer
	pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,		// additional config
		&pTarget
	);
	pBackBuffer->Release();
}

Graphics::~Graphics()
{

	if( pTarget != nullptr )
	{
		pTarget->Release();
	}
	// incase of failed init
	if (pContext != nullptr)
	{
		pContext->Release();
	}
	if (pSwap != nullptr)
	{
		pSwap->Release();
	}
	if (pDevice != nullptr)
	{
		pDevice->Release();
	}
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
				// |_UNIT SyncInterval Synchronize presentation after the nth vertical blank.
				// |_UNIT Flags An integer value that contains swap-chain presentation options
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue,1.0f };
	pContext->ClearRenderTargetView(pTarget, color);
}