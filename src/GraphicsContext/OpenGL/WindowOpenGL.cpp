#include "gpch.h"
#include "WindowOpenGL.h"

WindowOpenGL::WindowOpenGL(const int width, const int height, const char* const icon_path)
	: Window(width, height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

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
	glfwMakeContextCurrent(window);

	if (icon_path)
		this->SetWindowIcon(icon_path);

	glfwSwapInterval(1);
}

void WindowOpenGL::SwapBuffers()
{
	glfwSwapBuffers(window);
}
void WindowOpenGL::ToggleFullscreen()
{
	Window::ToggleFullscreen();
}
void WindowOpenGL::SetClearColor(const float red, const float green, const float blue, const float alpha) const
{
	glClearColor(red, green, blue, alpha);
}

void WindowOpenGL::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
