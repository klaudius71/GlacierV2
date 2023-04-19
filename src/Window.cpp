#include "gpch.h"
#include "Window.h"
#include "Application.h"
#include "Renderer2DAtt.h"
#include "RendererAtt.h"
#include "SceneManagerAtt.h"
#include "stb_image.h"

void Window::glfw_window_resize_callback(GLFWwindow* window, int width, int height)
{
	Window* glacier_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
	glacier_window->window_width = (width | 0x1) ^ 0x1; // Forces the values here to be even
	glacier_window->window_height = (height | 0x1) ^ 0x1;
#ifndef SHOW_EDITOR
	SceneManagerAtt::Callback::ScreenSizeChanged(width, height);
#endif
}
void Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	scancode; mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
		static_cast<Window*>(glfwGetWindowUserPointer(window))->ToggleFullscreen();
}

Window::Window(const int width, const int height)
	: prev_window_pos_x(0), prev_window_pos_y(0),
	prev_window_width(0), prev_window_height(0),
	window_width(width), window_height(height), 
	is_fullscreen(false)
{
}
Window::~Window()
{
	glfwTerminate();
}

const int Window::GetWindowWidth() const
{
	return window_width;
}
const int Window::GetWindowHeight() const
{
	return window_height;
}
GLFWwindow* const Window::GetGLFWWindow() const
{
	return window;
}
void Window::SetWindowTitle(const char* const name) const
{
	glfwSetWindowTitle(window, name);
}
void Window::HideCursor() const
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void Window::ShowCursor() const
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void Window::SetWindowIcon(const char* const icon_path)
{
	int x, y, channels;
	uint8_t* img = stbi_load(icon_path, &x, &y, &channels, 0);
	assert(img);
	GLFWimage glfwImg{ x, y, img };
	glfwSetWindowIcon(window, 1, &glfwImg);
	stbi_image_free(img);
}
NATIVE_WINDOW Window::GetNativeWindow() const
{
#if _WIN32
	return glfwGetWin32Window(window);
#else
	return 0;
#endif
}

const bool Window::IsOpen()
{
	return glfwWindowShouldClose(window);
}
void Window::PollEvents()
{
	glfwPollEvents();
}
void Window::ToggleFullscreen()
{
	if (is_fullscreen)
	{
		is_fullscreen = false;
		glfwSetWindowMonitor(window, nullptr, prev_window_pos_x, prev_window_pos_y, prev_window_width, prev_window_height, 0);
	}
	else
	{
		is_fullscreen = true;
		glfwGetWindowPos(window, &prev_window_pos_x, &prev_window_pos_y);
		glfwGetWindowSize(window, &prev_window_width, &prev_window_height);
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
		glfwSwapInterval(1);
	}
	
}