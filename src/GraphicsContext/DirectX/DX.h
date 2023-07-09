#ifndef _DX
#define _DX

class Window;

#if GLACIER_DIRECTX
class DX
{
private:
	static DX* instance;
	static DX& Instance()
	{
		assert(instance && "DirectX not initialized!");
		return *instance;
	}
	DX(const Window& window);
	DX(const DX&) = delete;
	DX& operator=(const DX&) = delete;
	DX(DX&&) = delete;
	DX& operator=(DX&&) = delete;
	~DX();

	float clear_color[4];
	ID3D11Device* dev = nullptr;
	ID3D11DeviceContext* devcon = nullptr;
	IDXGISwapChain* swapchain = nullptr;
	ID3D11RenderTargetView* backbuffer = nullptr;

	// Rasterizer States
	ID3D11RasterizerState* rasterizer_state = nullptr;
	ID3D11RasterizerState* rasterizer_state_front_cull = nullptr;
	ID3D11RasterizerState* rasterizer_state_no_cull = nullptr;

	// Depth Stencil States
	ID3D11DepthStencilState* depth_stencil_less = nullptr;
	ID3D11DepthStencilState* depth_stencil_lequal = nullptr;
	ID3D11DepthStencilView* depth_stencil_view = nullptr;

	// Blend state
	ID3D11BlendState* blend_state = nullptr;

	void setClearColor(const float red, const float green, const float blue, const float alpha);
	void clear();
	void swapBuffers();
	void resizeBuffers(const int width, const int height);

	void enableBlending();
	void disableBlending();

	void enableFrontFaceCulling();
	void enableBackFaceCulling();
	void disableCulling();

	void setDepthFunctionToLessEqual();
	void setDepthFunctionToLess();

	void setViewport(const float x, const float y, const float width, const float height, const float minDepth, const float maxDepth);

	void resetRenderTarget();

public:
	static void Initialize(const Window& window);

	static void SetClearColor(const float red, const float green, const float blue, const float alpha) { Instance().setClearColor(red, green, blue, alpha); }
	static void Clear() { Instance().clear(); }
	static void SwapBuffers() { Instance().swapBuffers(); }
	static void ResizeBuffers(const int width, const int height) { Instance().resizeBuffers(width, height); }

	static void EnableBlending() { Instance().enableBlending(); }
	static void DisableBlending() { Instance().disableBlending(); }

	static void EnableFrontFaceCulling() { Instance().enableFrontFaceCulling(); }
	static void EnableBackFaceCulling() { Instance().enableBackFaceCulling(); }
	static void DisableCulling() { Instance().disableCulling(); }

	static void SetDepthFunctionToLessEqual() { Instance().setDepthFunctionToLessEqual(); }
	static void SetDepthFunctionToLess() { Instance().setDepthFunctionToLess(); }

	static void SetViewport(const float x, const float y, const float width, const float height, const float minDepth = 0.0f, const float maxDepth = 1.0f) { Instance().setViewport(x, y, width, height, minDepth, maxDepth); }

	static void ResetRenderTarget() { Instance().resetRenderTarget(); }

	static IDXGISwapChain* const GetSwapchain() { return Instance().swapchain; }
	static ID3D11Device* const GetDevice() { return Instance().dev; }
	static ID3D11DeviceContext* const GetDeviceContext() { return Instance().devcon; }
	static ID3D11RenderTargetView* const GetBackBuffer() { return Instance().backbuffer; }

	static void Terminate();
};
#endif

#endif