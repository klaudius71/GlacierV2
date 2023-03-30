#include "gpch.h"
#include "DX.h"
#include "Window.h"

DX* DX::instance = nullptr;

DX::DX(const Window& window)
	: clear_color{ 0.471f, 0.694f, 0.749f, 1.0f }
{
#if GLACIER_DIRECTX
	swapchain = nullptr;
	dev = nullptr; 
	devcon = nullptr;
	backbuffer = nullptr;

	DXGI_SWAP_CHAIN_DESC scd{ 0 };
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window.GetNativeWindow();
	scd.SampleDesc.Count = 1;
	scd.Windowed = true;

	HRESULT result;
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, nullptr, &devcon);
	printf("%d\n", result);
	assert(swapchain);
	assert(dev);
	assert(devcon);

	ID3D11Texture2D* pBackBuffer = nullptr;
	swapchain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	assert(pBackBuffer);

	dev->CreateRenderTargetView(pBackBuffer, nullptr, &backbuffer);
	pBackBuffer->Release();
	assert(backbuffer);

	devcon->OMSetRenderTargets(1, &backbuffer, nullptr);

	D3D11_VIEWPORT viewport{ 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)window.GetWindowWidth();
	viewport.Height = (FLOAT)window.GetWindowHeight();

	devcon->RSSetViewports(1, &viewport);
#endif
}
DX::~DX()
{
#if GLACIER_DIRECTX
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
#endif
}

void DX::setClearColor(const float red, const float green, const float blue, const float alpha)
{
	clear_color[0] = red;
	clear_color[1] = green;
	clear_color[2] = blue;
	clear_color[3] = alpha;
}
void DX::clear()
{
#if GLACIER_DIRECTX
	devcon->ClearRenderTargetView(backbuffer, clear_color);
#endif
}

void DX::swapBuffers()
{
#if GLACIER_DIRECTX
	swapchain->Present(1, 0);
#endif
}

void DX::Initialize(const Window& window)
{
	assert(!instance && "DirectX already initialized!");
	instance = new DX(window);
}
void DX::Terminate()
{
	assert(instance && "DirectX not initialized!");
	delete instance;
}