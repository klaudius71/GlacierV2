#include "gpch.h"
#include "Window.h"
#include "Glacier.h"
#include "Window.h"

void Window::glfw_window_resize_callback(GLFWwindow* window, int width, int height)
{
	Window* glacier_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
	glacier_window->window_width = width;
	glacier_window->window_height = height;
}
void Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	scancode; mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
		static_cast<Window*>(glfwGetWindowUserPointer(window))->ToggleFullscreen();
}

Window::Window(const int& width, const int& height)
	: window_width(width), window_height(height), is_fullscreen(false)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
	
	window = glfwCreateWindow(width, height, "Glacier V2", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		assert(false && "Failed to create GLFW window!");
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
	glfwSetKeyCallback(window, glfw_key_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetWindowPos(window, &prev_window_pos_x, &prev_window_pos_y);

	//glfwSwapInterval(0);
}
Window::~Window()
{
	glfwTerminate();
}

const int& Window::GetWindowWidth() const
{
	return window_width;
}
const int& Window::GetWindowHeight() const
{
	return window_height;
}
void Window::SetWindowTitle(const char* const name) const
{
	glfwSetWindowTitle(window, name);
}
void Window::SetClearColor(const float& red, const float& green, const float& blue, const float& alpha) const
{
	glClearColor(red, green, blue, alpha);
}
GLFWwindow* const Window::GetNativeWindow() const
{
	return window;
}

const bool Window::IsOpen()
{
	return glfwWindowShouldClose(window);
}
void Window::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
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
		glfwSetWindowMonitor(window, nullptr, prev_window_pos_x, prev_window_pos_y, 1280, 720, 0);
	}
	else
	{
		is_fullscreen = true;
		glfwGetWindowPos(window, &prev_window_pos_x, &prev_window_pos_y);
		int monitor_count;
		GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
		const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);
		glfwSetWindowMonitor(window, monitors[0], 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
	}
	
}
