#include "gpch.h"
#include "Lighting.h"
#include "Scene.h"
#include "Components.h"

GLuint Lighting::DirLight_ubo = 0;

void Lighting::SetBuffers(const GLuint& dir_light_ubo)
{
	DirLight_ubo = dir_light_ubo;
}

void Lighting::UpdateBuffers(const Scene& curr_scene)
{
	const entt::registry& scene_registry = curr_scene.GetRegistry();

	auto dir_light_view = scene_registry.view<DirectionalLightComponent>();
	auto temp = dir_light_view.begin();
	temp;
	if (dir_light_view.begin() != dir_light_view.end())
	{
		glBindBuffer(GL_UNIFORM_BUFFER, DirLight_ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(VertexTypes::DirectionalLight), &scene_registry.get<DirectionalLightComponent>(*dir_light_view.begin()).light);
	}
}
