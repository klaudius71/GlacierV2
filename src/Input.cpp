#include "gpch.h"
#include "Input.h"
#include "Glacier.h"
#include "Window.h"

glm::f64vec2 Input::curr_mouse_position = glm::f64vec2(0.0f);
glm::f64vec2 Input::prev_mouse_position = glm::f64vec2(0.0f);
glm::vec2 Input::mouse_delta = glm::vec2(0.0f);

bool Input::GetKeyDown(GLACIER_KEY key)
{
	auto state = glfwGetKey(Glacier::GetWindow().GetNativeWindow(), (int)key);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::GetMouseButtonState(GLACIER_MOUSE btn)
{
	auto state = glfwGetMouseButton(Glacier::GetWindow().GetNativeWindow(), (int)btn);
	return state == GLFW_PRESS;
}
const glm::f64vec2& Input::GetMousePosition()
{
	return curr_mouse_position;
}
const glm::vec2& Input::GetMouseDeltaPosition()
{
	return mouse_delta;
}

void Input::ProcessMouseData()
{
	glfwGetCursorPos(Glacier::GetWindow().GetNativeWindow(), &curr_mouse_position.x, &curr_mouse_position.y);
	mouse_delta = prev_mouse_position - curr_mouse_position;
	mouse_delta.x = -mouse_delta.x;
	prev_mouse_position = curr_mouse_position;
}
