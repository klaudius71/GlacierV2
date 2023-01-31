#ifndef _WINDOW
#define _WINDOW

#include "GlacierCore.h"

class GLACIER_API Window
{
public:
	Window(const int& width = 1280, const int& height = 720);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;
	~Window();

	const int& GetWindowWidth() const;
	const int& GetWindowHeight() const;
	void SetWindowTitle(const char* const name) const;
	void SetClearColor(const float& red, const float& green, const float& blue, const float& alpha = 1.0f) const;
	GLFWwindow* const GetNativeWindow() const;

	const bool IsOpen();
	void Clear();
	void SwapBuffers();
	void PollEvents();
	void ToggleFullscreen();

private:
	static void glfw_window_resize_callback(GLFWwindow* window, int width, int height);
	static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	int prev_window_pos_x;
	int prev_window_pos_y;
	int window_width;
	int window_height;
	GLFWwindow* window;
	bool is_fullscreen;
};

#endif _WINDOW