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
	scd.BufferCount = 2;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window.GetNativeWindow();
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = (UINT)D3D11_CENTER_MULTISAMPLE_PATTERN;
	scd.Windowed = true;

	UINT createDeviceFlags = 0;
#if _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, nullptr, &devcon);
	assert(swapchain);
	assert(dev);
	assert(devcon);

	ID3D11Texture2D* pBackBuffer = nullptr;
	swapchain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	assert(pBackBuffer);

	dev->CreateRenderTargetView(pBackBuffer, nullptr, &backbuffer);
	pBackBuffer->Release();
	assert(backbuffer);

	// Rasterizer
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_FRONT;
	rd.FrontCounterClockwise = true;
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true; 
	rd.AntialiasedLineEnable = false;

	dev->CreateRasterizerState(&rd, &rasterizer_state);
	devcon->RSSetState(rasterizer_state);

	// Depth stencil
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = window.GetWindowWidth();
	descDepth.Height = window.GetWindowHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = (UINT)D3D11_CENTER_MULTISAMPLE_PATTERN;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* depthStencilTexture;
	hr = dev->CreateTexture2D(&descDepth, NULL, &depthStencilTexture);
	assert(SUCCEEDED(hr));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;

	hr = dev->CreateDepthStencilView(depthStencilTexture, &descDSV, &depth_stencil_view);
	assert(SUCCEEDED(hr));
	depthStencilTexture->Release();

	// Set the render target, including the depth stencil
	devcon->OMSetRenderTargets(1, &backbuffer, depth_stencil_view);

	// Create a blend state
	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.AlphaToCoverageEnable = true;
	blendStateDesc.IndependentBlendEnable = true;
	blendStateDesc.RenderTarget[0] = rtbd;
	hr = dev->CreateBlendState(&blendStateDesc, &blend_state);
	assert(SUCCEEDED(hr));

	// Setting the viewport
	D3D11_VIEWPORT viewport{ 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)window.GetWindowWidth();
	viewport.Height = (FLOAT)window.GetWindowHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	devcon->RSSetViewports(1, &viewport);
#endif
}
DX::~DX()
{
#if GLACIER_DIRECTX
	blend_state->Release();
	depth_stencil_view->Release();
	rasterizer_state->Release();
	swapchain->Release();
	backbuffer->Release();
	devcon->Release();

#ifdef _DEBUG
	HRESULT hr;
	ID3D11Debug* debugDev;
	hr = dev->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));
	assert(SUCCEEDED(hr));

	debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	debugDev->Release();
#endif

	dev->Release();
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
	devcon->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
#endif
}

void DX::swapBuffers()
{
#if GLACIER_DIRECTX
	swapchain->Present(1, 0);
#endif
}

void DX::enableBlending()
{
#if GLACIER_DIRECTX
	devcon->OMSetBlendState(blend_state, nullptr, 0xFFFFFFFF);
#endif
}
void DX::disableBlending()
{
#if GLACIER_DIRECTX
	devcon->OMSetBlendState(nullptr, nullptr, 0xFFFFFFF);
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