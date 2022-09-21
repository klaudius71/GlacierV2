#include "gpch.h"
#include "Lighting.h"
#include "Scene.h"
#include "Components.h"

GLuint Lighting::DirLight_ubo = 0;
const DirectionalLightComponent Lighting::default_dir_light = DirectionalLightComponent(VertexTypes::PhongADS(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f), glm::vec3(0.0f));

void Lighting::SetBuffers(const GLuint& dir_light_ubo)
{
	DirLight_ubo = dir_light_ubo;
}

void Lighting::UpdateBuffers(const Scene& curr_scene)
{
	const entt::registry& scene_registry = curr_scene.GetRegistry();

	glBindBuffer(GL_UNIFORM_BUFFER, DirLight_ubo);
	auto dir_light_view = scene_registry.view<DirectionalLightComponent>();
	if (dir_light_view.begin() != dir_light_view.end())
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(VertexTypes::DirectionalLight), &scene_registry.get<DirectionalLightComponent>(*dir_light_view.begin()).light);
	else
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(VertexTypes::DirectionalLight), &default_dir_light);
	
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
