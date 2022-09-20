#include "gpch.h"
#include "Renderer.h"
#include "Scene.h"
#include "Components.h"
#include "Glacier.h"
#include "Window.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"

void Renderer::UpdateCameraData(const CameraComponent& camera)
{
	const GLuint& ubo = ShaderLoader::GetMatricesUBO();
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraComponent) /*- sizeof(glm::vec4)*/, &camera);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::RenderScene(Scene* const scn)
{
	entt::registry& registry = scn->GetRegistry();
	
	const CameraComponent& camera = scn->GetActiveCamera();
	UpdateCameraData(camera);

	auto curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::TEXTURE_LIT)->GetProgramID();
	glUseProgram(curr_shader);
	GLint world_matrix_uniform_loc = glGetUniformLocation(curr_shader, "world_matrix");
	GLint material_ambient_uniform_loc = glGetUniformLocation(curr_shader, "material.ambient");
	GLint material_diffuse_uniform_loc = material_ambient_uniform_loc + 1;
	GLint material_specular_uniform_loc = material_diffuse_uniform_loc + 1;
	GLint color_uniform_loc = glGetUniformLocation(curr_shader, "color");
	const GLint tex[4] = { 0, 1, 2, 3 };
	glUniform1iv(glGetUniformLocation(curr_shader, "textures[0]"), 4, tex);
	
	// Render meshes with materials
	auto render_group_material = registry.group<MaterialComponent, MeshComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, material, mesh, transform] : render_group_material.each())
	{
		glUniform3fv(material_ambient_uniform_loc,  1, (const GLfloat*)&material.ads.ambient);
		glUniform3fv(material_diffuse_uniform_loc,  1, (const GLfloat*)&material.ads.diffuse);
		glUniform4fv(material_specular_uniform_loc, 1, (const GLfloat*)&material.ads.specular);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.tex_id.s);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material.tex_id.t);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material.tex_id.p);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, material.tex_id.q);
		glUniform4fv(color_uniform_loc, 1, (const GLfloat*)&material.col);
		glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&transform.transform);
		glBindVertexArray(mesh.vao);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);
	}

	// Render meshes without materials
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::COLOR)->GetProgramID();
	glUseProgram(curr_shader);
	world_matrix_uniform_loc = glGetUniformLocation(curr_shader, "world_matrix");
	glUniform4fv(glGetUniformLocation(curr_shader, "color"), 1, (const GLfloat*)&Colors::White);
	auto render_group = registry.group<>(entt::get<MeshComponent, TransformComponent>, entt::exclude<MaterialComponent>);
	for (auto&& [entity, mesh, transform] : render_group.each())
	{
		glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&transform.GetWorldMatrix());
		glBindVertexArray(mesh.vao);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
