#include "gpch.h"
#include "Renderer.h"
#include "Scene.h"
#include "Components.h"
#include "Application.h"
#include "Window.h"
#include "ShaderLoader.h"
#include "Shader.h"
#include "TextureLoader.h"
#include "ModelLoader.h"
#include "Lighting.h"
#include "Animator.h"
#include "SkeletalAnimation.h"
#include "Logger.h"

Renderer* Renderer::instance = nullptr;
Renderer::Renderer()
	: main_framebuffer(Glacier::GetWindow().GetWindowWidth(), Glacier::GetWindow().GetWindowHeight())
{
}

void Renderer::UpdateCameraData(const CameraComponent& camera)
{
	const GLuint& ubo = ShaderLoader::GetMatricesUBO();
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	const glm::mat4 proj_view[2] = { camera.proj, glm::lookAt(camera.cam_pos, camera.cam_pos + camera.cam_dir, glm::vec3(0.0f, 1.0f, 0.0f))};
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4) * 2, &proj_view);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Renderer::UpdateViewportSize(const int& width, const int& height)
{
	assert(instance);
	instance->main_framebuffer.Resize(width, height);
}

void Renderer::RenderLit(Scene& scn)
{
	entt::registry& registry = scn.GetRegistry();

	GLuint curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::TEXTURE_LIT)->GetProgramID();
	glUseProgram(curr_shader);
	GLint world_matrix_uniform_loc = glGetUniformLocation(curr_shader, "world_matrix");
	GLint material_ambient_uniform_loc = glGetUniformLocation(curr_shader, "material.ambient");
	GLint material_diffuse_uniform_loc = material_ambient_uniform_loc + 1;
	GLint material_specular_uniform_loc = material_diffuse_uniform_loc + 1;
	GLint color_uniform_loc = glGetUniformLocation(curr_shader, "color");
	glUniform1iv(glGetUniformLocation(curr_shader, "textures[0]"), 4, std::array<GLint, 4>({ 0, 1, 2, 3 }).data());
	glUniform1i(glGetUniformLocation(curr_shader, "normal_map"), 4);
	glUniform1i(glGetUniformLocation(curr_shader, "dir_shadow_map"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, Lighting::DirShadow_tex);

	// Render meshes with materials
	auto render_group_material = registry.group<MaterialComponent, MeshComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, material, mesh, transform] : render_group_material.each())
	{
		glUniform3fv(material_ambient_uniform_loc, 1, (const GLfloat*)&material.ads.ambient);
		glUniform3fv(material_diffuse_uniform_loc, 1, (const GLfloat*)&material.ads.diffuse);
		glUniform4fv(material_specular_uniform_loc, 1, (const GLfloat*)&material.ads.specular);
		for (int i = 0; i < 4; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, material.tex_id[i]);
		}
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, material.norm_tex_id[0]);
		glUniform4fv(color_uniform_loc, 1, (const GLfloat*)&material.col);
		glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&transform.GetWorldMatrix());
		glBindVertexArray(mesh.vao);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);
	}
}
void Renderer::RenderSkinned(Scene& scn)
{
	entt::registry& registry = scn.GetRegistry();
	
	GLuint curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::TEXTURE_SKINNED_LIT)->GetProgramID();
	glUseProgram(curr_shader);
	const GLint bone_matrices_uniform_loc = glGetUniformLocation(curr_shader, "bone_matrices[0]");
	const GLint world_matrix_uniform_loc = glGetUniformLocation(curr_shader, "world_matrix");
	const GLint material_ambient_uniform_loc = glGetUniformLocation(curr_shader, "material.ambient");
	const GLint material_diffuse_uniform_loc = material_ambient_uniform_loc + 1;
	const GLint material_specular_uniform_loc = material_diffuse_uniform_loc + 1;
	const GLint color_uniform_loc = glGetUniformLocation(curr_shader, "color");
	glUniform1iv(glGetUniformLocation(curr_shader, "textures[0]"), 4, std::array<GLint, 4>({ 0, 1, 2, 3 }).data());
	glUniform1i(glGetUniformLocation(curr_shader, "normal_map"), 4);
	glUniform1i(glGetUniformLocation(curr_shader, "dir_shadow_map"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, Lighting::DirShadow_tex);

	// Render meshes with materials
	glDisable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	auto render_group = registry.group<SkeletalMeshComponent>(entt::get<TransformComponent, MaterialComponent>);
	for (auto&& [entity, skel_mesh, transform, material] : render_group.each())
	{
		glUniform3fv(material_ambient_uniform_loc, 1, (const GLfloat*)&material.ads.ambient);
		glUniform3fv(material_diffuse_uniform_loc, 1, (const GLfloat*)&material.ads.diffuse);
		glUniform4fv(material_specular_uniform_loc, 1, (const GLfloat*)&material.ads.specular);
		for (int i = 0; i < 4; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, material.tex_id[i]);
		}
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, material.norm_tex_id[0]);
		glUniform4fv(color_uniform_loc, 1, (const GLfloat*)&material.col);
		glUniformMatrix4fv(bone_matrices_uniform_loc, skel_mesh.num_joints, GL_FALSE, (const GLfloat*)skel_mesh.bone_matrices);
		glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&transform.GetWorldMatrix());
		glBindVertexArray(skel_mesh.vao);
		glDrawElements(GL_TRIANGLES, skel_mesh.num_indices, GL_UNSIGNED_INT, nullptr);
	}
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}
void Renderer::RenderUnlit(Scene& scn)
{
	// Render meshes without materials
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	const GLuint& shader = ShaderLoader::Get(PRELOADED_SHADERS::COLOR)->GetProgramID();
	glUseProgram(shader);
	GLint world_matrix_uniform_loc = glGetUniformLocation(shader, "world_matrix");
	glUniform4fv(glGetUniformLocation(shader, "color"), 1, (const GLfloat*)&Colors::White);
	auto render_group = scn.GetRegistry().group(entt::get<MeshComponent, TransformComponent>, entt::exclude<MaterialComponent>);
	for (auto&& [entity, mesh, transform] : render_group.each())
	{
		glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&transform.GetWorldMatrix());
		glBindVertexArray(mesh.vao);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void Renderer::RenderSkybox(Scene& scn)
{
	// Render skybox
	if (SkyboxComponent* skybox = scn.GetFirstComponent<SkyboxComponent>())
	{
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_FRONT);

		const GLuint& curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::SKYBOX)->GetProgramID();
		glUseProgram(curr_shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->tex_id);
		Model* skybox_model = ModelLoader::Get(PRELOADED_MODELS::UNIT_CUBE);
		glBindVertexArray(skybox_model->GetVAO());
		glDrawElements(GL_TRIANGLES, skybox_model->GetNumTriangles() * 3, GL_UNSIGNED_INT, nullptr);

		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);
	}
}

const Framebuffer& Renderer::GetMainFramebuffer()
{
	assert(instance);
	return instance->main_framebuffer;
}

void Renderer::CullScene(Scene& scn, const CameraComponent& camera)
{	
	UNREFERENCED_PARAMETER(camera);

	entt::registry& registry = scn.GetRegistry();

	auto transform_mesh_view = registry.view<TransformComponent, MeshComponent>();
	for (auto&& [entity, transform, mesh] : transform_mesh_view.each())
	{
		
	}
}
void Renderer::RenderScene(Scene& scn)
{
	GLACIER_LOG_FUNC_TIMER("3d");

	const CameraComponent& camera = scn.GetActiveCamera();
	UpdateCameraData(camera);

	CullScene(scn, camera);

	Lighting::RenderSceneShadows(&scn, camera);
	
	const Framebuffer& framebuffer = instance->main_framebuffer;
	framebuffer.Bind();
	glViewport(0, 0, framebuffer.GetSize().x, framebuffer.GetSize().y);

	RenderLit(scn);
	RenderSkinned(scn);
	RenderUnlit(scn);

	RenderSkybox(scn);

	framebuffer.Unbind();
	const Window& window = Glacier::GetWindow();
	glViewport(0, 0, window.GetWindowWidth(), window.GetWindowHeight());
}

void Renderer::Initialize()
{
	assert(!instance);
	instance = new Renderer;
}
void Renderer::Terminate()
{
	delete instance;
	instance = nullptr;
}