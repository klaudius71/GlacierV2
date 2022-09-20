#include "gpch.h"
#include "RotatingScript.h"

void RotatingScript::OnCreate()
{
	transform = &obj->GetComponent<TransformComponent>();
}
void RotatingScript::OnTick()
{
	transform->rotation() = glm::vec3(cosf((float)glfwGetTime()), 1.0f, sinf((float)glfwGetTime()));
	//glm::vec4& curr_pos = (*transform)[3];
	//curr_pos.y = sinf((float)glfwGetTime() * 2.0f) * 10.0f + 60.0f;
}