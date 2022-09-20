#include "gpch.h"
#include "SpinningLightScript.h"

void SpinningLightScript::OnCreate()
{
	dir = &obj->GetComponent<DirectionalLightComponent>().light.direction;
}

void SpinningLightScript::OnTick()
{
	*dir = glm::vec3(glm::rotate(glm::mat4(1.0f), TimeManager::GetDeltaTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(*dir, 0.0f));
}
