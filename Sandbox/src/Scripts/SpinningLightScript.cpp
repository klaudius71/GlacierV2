#include "SpinningLightScript.h"

SpinningLightScript::SpinningLightScript()
	: Script("SpinningLightScript")
{
}

void SpinningLightScript::OnSceneEnter()
{
	dir = &GetGameObject()->GetComponent<DirectionalLightComponent>().light.direction;
}

void SpinningLightScript::OnUpdate(float dt)
{
	*dir = glm::vec3(glm::rotate(glm::mat4(1.0f), dt * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(*dir, 0.0f));
}
