#include "gpch.h"
#include "Window.h"
#include "Glacier.h"
#include "Window.h"

void Window::glfw_window_resize_callback(GLFWwindow* window, int width, int height)
{
	window;
	glViewport(0, 0, width, height);
}
void Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	scancode; mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Window::Window(const int& width, const int& height)
	: window_width(width), window_height(height)
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
	glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
	glfwSetKeyCallback(window, glfw_key_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(0);
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