#include "gpch.h"
#include "WindowDirectX.h"
#include "DX.h"

WindowDirectX::WindowDirectX(const int width, const int height, const char* const icon_path)
	: Window(width, height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, "Glacier V2", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		assert(false && "Failed to create GLFW window!");
	}

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
	glfwSetKeyCallback(window, glfw_key_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetWindowPos(window, &prev_window_pos_x, &prev_window_pos_y);

	if (icon_path)
		this->SetWindowIcon(icon_path);
}

#if GLACIER_DIRECTX
void WindowDirectX::SetClearColor(const float red, const float green, const float blue, const float alpha) const
{
	DX::SetClearColor(red, green, blue, alpha);
}
void WindowDirectX::Clear()
{
	DX::Clear();
}
void WindowDirectX::SwapBuffers()
{
	DX::SwapBuffers();
}
void WindowDirectX::ToggleFullscreen()
{
	Window::ToggleFullscreen();

	// Something DX related to fullscreen
}
#elif GLACIER_OPENGL
void WindowDirectX::SetClearColor(const float, const float, const float, const float) const
{
}
void WindowDirectX::Clear()
{
}
void WindowDirectX::SwapBuffers()
{
}
void WindowDirectX::ToggleFullscreen()
{
}
#endif