#ifndef _DX
#define _DX

class Window;

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
#if GLACIER_DIRECTX
	IDXGISwapChain* swapchain = nullptr;
	ID3D11Device* dev = nullptr;
	ID3D11DeviceContext* devcon = nullptr;
	ID3D11RenderTargetView* backbuffer = nullptr;
#endif

	void setClearColor(const float red, const float green, const float blue, const float alpha);
	void clear();
	void swapBuffers();

public:
	static void Initialize(const Window& window);

	static void SetClearColor(const float red, const float green, const float blue, const float alpha) { Instance().setClearColor(red, green, blue, alpha); }
	static void Clear() { Instance().clear(); }
	static void SwapBuffers() { Instance().swapBuffers(); }

#if GLACIER_DIRECTX
	static IDXGISwapChain* const GetSwapchain() { return Instance().swapchain; }
	static ID3D11Device* const GetDevice() { return Instance().dev; }
	static ID3D11DeviceContext* const GetDeviceContext() { return Instance().devcon; }
	static ID3D11RenderTargetView* const GetBackBuffer() { return Instance().backbuffer; }
#endif

	static void Terminate();
};

#endif