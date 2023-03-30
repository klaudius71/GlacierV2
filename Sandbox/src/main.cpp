#include <stdio.h>
#include "Sandbox.h"

//#define OPENGL

#ifndef OPENGL
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")
#endif

struct MemMarker {
	~MemMarker()
	{
	}
} mem_marker;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#ifdef _SHOW_CONSOLE
	AllocConsole();
	FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;
	freopen_s(&fpstdin, "CONIN$", "r", stdin);
	freopen_s(&fpstdout, "CONOUT$", "w", stdout);
	freopen_s(&fpstderr, "CONOUT$", "w", stderr);
#endif

#ifdef OPENGL
	Sandbox* sandbox = new Sandbox(1920, 1080);
	sandbox->Run();
	delete sandbox;
	return 0;
#else

	Window* window = new Window(1280, 720);

	IDXGISwapChain* swapchain = nullptr;
	ID3D11Device* dev = nullptr;
	ID3D11DeviceContext* devcon = nullptr;
	ID3D11RenderTargetView* backbuffer = nullptr;
	
	DXGI_SWAP_CHAIN_DESC scd{ 0 };
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window->GetWin32Window();
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
	viewport.Width = (FLOAT)window->GetWindowWidth();
	viewport.Height = (FLOAT)window->GetWindowHeight();

	devcon->RSSetViewports(1, &viewport);

	while (!window->IsOpen())
	{
		const FLOAT clear_color[4] = { 0.471f, 0.694f, 0.749f, 1.0f };
		devcon->ClearRenderTargetView(backbuffer, clear_color);
		swapchain->Present(1, 0);

		window->PollEvents();
	}

	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();

	delete window;

#endif
}