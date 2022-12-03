#include "gpch.h"
#include "Lighting.h"
#include "Scene.h"
#include "Components.h"
#include "ShaderLoader.h"
#include "Shader.h"
#include "Glacier.h"
#include "Window.h"

GLuint Lighting::DirLight_ubo = 0;
GLuint Lighting::LightspaceMatrices_ubo = 0;

GLuint Lighting::DirShadow_fbo = 0;
GLuint Lighting::DirShadow_tex = 0;
const DirectionalLightComponent Lighting::default_dir_light = DirectionalLightComponent(VertexTypes::PhongADS(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f), glm::vec3(0.0f));

void Lighting::SetBuffers(const GLuint& dir_light_ubo, const GLuint& lightspace_ubo)
{
	DirLight_ubo = dir_light_ubo;
	LightspaceMatrices_ubo = lightspace_ubo;

	glGenFramebuffers(1, &DirShadow_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, DirShadow_fbo);
	
	glGenTextures(1, &DirShadow_tex);
	glBindTexture(GL_TEXTURE_2D, DirShadow_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DIR_SHADOW_MAP_SIZE, DIR_SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (const GLfloat*) & Colors::White);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DirShadow_tex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void Lighting::RenderSceneShadows(Scene* const curr_scene, const CameraComponent& cam)
{
	entt::registry& scene_registry = curr_scene->GetRegistry();

	auto dir_light = curr_scene->GetFirstComponent<DirectionalLightComponent>();
	if (!dir_light)
		return;

	glViewport(0, 0, DIR_SHADOW_MAP_SIZE, DIR_SHADOW_MAP_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, DirShadow_fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_FRONT);

	auto& dir_light_dir = dir_light->light.direction;
	const glm::vec3 dir_light_cam_center_pos = cam.cam_pos + glm::normalize(cam.cam_dir) * 256.0f;
	const glm::mat4 lightspace = glm::ortho(-512.0f, 512.0f, -512.0f, 512.0f, -512.0f, 512.0f) *
		glm::lookAt(dir_light_cam_center_pos - dir_light_dir * 128.0f, dir_light_cam_center_pos, glm::vec3(0.0f, 1.0f, 0.0f));

	glBindBuffer(GL_UNIFORM_BUFFER, LightspaceMatrices_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &lightspace);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLuint curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::SHADOW_MAP)->GetProgramID();
	glUseProgram(curr_shader);

	GLint world_matrix_uniform_loc = glGetUniformLocation(curr_shader, "world_matrix");
	auto mesh_transform_group = scene_registry.group<MeshComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, mesh, transform] : mesh_transform_group.each())
	{
		if (mesh.cast_shadow)
		{
			glBindVertexArray(mesh.vao);
			glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&transform.GetWorldMatrix());
			glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);
		}
	}

	curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::SHADOW_MAP_SKINNED)->GetProgramID();
	glUseProgram(curr_shader);
	world_matrix_uniform_loc = glGetUniformLocation(curr_shader, "world_matrix");
	GLint bone_matrices_uniform_loc = glGetUniformLocation(curr_shader, "bone_matrices[0]");
	auto skel_mesh_anim_group = scene_registry.group<SkeletalMeshComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, skel_mesh, transform] : skel_mesh_anim_group.each())
	{
		if (skel_mesh.cast_shadow)
		{
			glBindVertexArray(skel_mesh.mod->GetVAO());
			glUniformMatrix4fv(bone_matrices_uniform_loc, skel_mesh.num_bones, GL_FALSE, (const GLfloat*)skel_mesh.bone_matrices);
			glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&transform.GetWorldMatrix());
			glDrawElements(GL_TRIANGLES, skel_mesh.num_indices, GL_UNSIGNED_INT, nullptr);
		}
	}

	//glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Glacier::GetWindow().GetWindowWidth(), Glacier::GetWindow().GetWindowHeight());

	ImGui::Begin("Debug");
	ImGui::Text("DirLight Shadow Texture:");
	ImGui::Image((ImTextureID)(uint64_t)DirShadow_tex, ImVec2{256, 256}, ImVec2{0,0}, ImVec2{1,1});
	ImGui::End();
}