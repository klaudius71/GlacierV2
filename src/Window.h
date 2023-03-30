#ifndef _WINDOW
#define _WINDOW

#include "GlacierCore.h"

class GLACIER_API Window
{
protected:
	Window(const int width, const int height);
public:
	Window() = delete;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;
	virtual ~Window();

	const int GetWindowWidth() const;
	const int GetWindowHeight() const;
	GLFWwindow* const GetGLFWWindow() const;
	void SetWindowTitle(const char* const name) const;
	void HideCursor() const;
	void ShowCursor() const;

	void SetWindowIcon(const char* const icon_path);
	NATIVE_WINDOW GetNativeWindow() const;

	const bool IsOpen();
	void PollEvents();

	virtual void SetClearColor(const float red, const float green, const float blue, const float alpha = 1.0f) const = 0;
	virtual void Clear() = 0;
	virtual void SwapBuffers() = 0;
	virtual void ToggleFullscreen() = 0;

protected:
	static void glfw_window_resize_callback(GLFWwindow* window, int width, int height);
	static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	int prev_window_pos_x;
	int prev_window_pos_y;
	int prev_window_width;
	int prev_window_height;
	int window_width;
	int window_height;
	GLFWwindow* window;
	bool is_fullscreen;
};

#endif _WINDOW