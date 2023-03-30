#ifndef _WINDOW_DIRECTX
#define _WINDOW_DIRECTX

#include "Window.h"

class GLACIER_API WindowDirectX : public Window
{
public:
	WindowDirectX(const int width = 1280, const int height = 720, const char* const icon_path = nullptr);
	WindowDirectX(const WindowDirectX&) = delete;
	WindowDirectX& operator=(const WindowDirectX&) = delete;
	WindowDirectX(WindowDirectX&&) = delete;
	WindowDirectX& operator=(WindowDirectX&&) = delete;
	~WindowDirectX() = default;

	virtual void SetClearColor(const float red, const float green, const float blue, const float alpha = 1.0f) const override;
	virtual void Clear() override;
	virtual void SwapBuffers() override;
	virtual void ToggleFullscreen() override;
};

#endif