#ifndef _WINDOW_OPENGL
#define _WINDOW_OPENGL

#include "Window.h"

class GLACIER_API WindowOpenGL : public Window
{
public:
	WindowOpenGL(const int width = 1280, const int height = 720, const char* const icon_path = nullptr);
	WindowOpenGL(const WindowOpenGL&) = delete;
	WindowOpenGL& operator=(const WindowOpenGL&) = delete;
	WindowOpenGL(WindowOpenGL&&) = delete;
	WindowOpenGL& operator=(WindowOpenGL&&) = delete;
	~WindowOpenGL() = default;

	virtual void SetClearColor(const float red, const float green, const float blue, const float alpha = 1.0f) const override;
	virtual void Clear() override;
	virtual void SwapBuffers() override;
	virtual void ToggleFullscreen() override;
};

#endif