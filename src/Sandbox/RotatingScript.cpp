#include "gpch.h"
#include "RotatingScript.h"

RotatingScript::RotatingScript(const glm::vec3& rot_direction)
	: rot_direction(rot_direction)
{
}

void RotatingScript::OnCreate()
{
	transform = &GetGameObject()->GetComponent<TransformComponent>();
}
void RotatingScript::OnTick()
{
	transform->rotation() += rot_direction * TimeManager::GetDeltaTime();
	//glm::vec4& curr_pos = (*transform)[3];
	//curr_pos.y = sinf((float)glfwGetTime() * 2.0f) * 10.0f + 60.0f;
}