#include "gpch.h"
#include "RotatingScript.h"

RotatingScript::RotatingScript(const glm::vec3& rot_direction)
	: Script("RotatingScript"), rot_direction(rot_direction)
{
}

void RotatingScript::OnSceneEnter()
{
	transform = &GetGameObject()->GetComponent<TransformComponent>();
}
void RotatingScript::OnUpdate(float dt)
{
	transform->rotation() += rot_direction * dt;
	//glm::vec4& curr_pos = (*transform)[3];
	//curr_pos.y = sinf((float)glfwGetTime() * 2.0f) * 10.0f + 60.0f;
}