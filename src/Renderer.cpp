#include "gpch.h"
#include "Renderer.h"
#include "Scene.h"
#include "Components.h"
#include "Application.h"
#include "Window.h"
#include "ShaderLoader.h"
#include "ShaderOpenGL.h"
#include "TextureLoader.h"
#include "ModelLoader.h"
#include "Lighting.h"
#include "Animator.h"
#include "SkeletalAnimation.h"
#include "Logger.h"
#include "DX.h"
#include "ConstantBuffer.h"

Renderer* Renderer::instance = nullptr;

Renderer::Renderer()
	: main_framebuffer(Glacier::GetWindow().GetWindowWidth(), Glacier::GetWindow().GetWindowHeight())
{
}

void Renderer::UpdateCameraData(const CameraComponent& camera)
{
#if GLACIER_OPENGL
	const GLuint& ubo = ShaderLoader::GetMatricesUBO();
	const glm::mat4 proj_view[2] = { camera.proj, glm::lookAt(camera.cam_pos, camera.cam_pos + camera.cam_dir, glm::vec3(0.0f, 1.0f, 0.0f))};
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4) * 2, &proj_view);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
#elif GLACIER_DIRECTX
	const VertexTypes::CamData CamData{ camera.proj, glm::lookAt(camera.cam_pos, camera.cam_pos + camera.cam_dir, glm::vec3(0.0f, 1.0f, 0.0f)) };
	ShaderLoader::GetCamDataConstantBuffer()->UpdateData(DX::GetDeviceContext(), &CamData, sizeof(VertexTypes::CamData));
#endif
}
void Renderer::UpdateViewportSize(const int width, const int height)
{
	assert(instance);
	instance->main_framebuffer.Resize(width, height);
}

void Renderer::RenderLit(Scene& scn)
{
#if GLACIER_OPENGL
	entt::registry& registry = scn.GetRegistry();

	auto curr_shader = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::TEXTURE_LIT);
	curr_shader->Bind();
	GLint world_matrix_uniform_loc = curr_shader->GetUniformLocation("world_matrix");
	GLint material_ambient_uniform_loc = curr_shader->GetUniformLocation("material.ambient");
	GLint material_diffuse_uniform_loc = material_ambient_uniform_loc + 1;
	GLint material_specular_uniform_loc = material_diffuse_uniform_loc + 1;
	GLint color_uniform_loc = curr_shader->GetUniformLocation("color");
	glUniform1iv(curr_shader->GetUniformLocation("textures[0]"), 4, std::array<GLint, 4>({ 0, 1, 2, 3 }).data());
	glUniform1i(curr_shader->GetUniformLocation("normal_map"), 4);
	glUniform1i(curr_shader->GetUniformLocation("dir_shadow_map"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, Lighting::Instance().DirShadow_tex);

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
#elif GLACIER_DIRECTX
	entt::registry& registry = scn.GetRegistry();

	// Get the DX variables
	auto devcon = DX::GetDeviceContext();
	auto instance_data_cbuffer = ShaderLoader::GetInstanceDataConstantBuffer();
	auto material_data_cbuffer = ShaderLoader::GetMaterialDataConstantBuffer();

	auto curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::TEXTURE_LIT);
	curr_shader->Bind();

	Lighting::BindShadowDepthTexture(2);

	// Render meshes with materials
	auto render_group_material = registry.group<MaterialComponent, MeshComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, material, mesh, transform] : render_group_material.each())
	{
		material.tex->Bind(0);
		material.norm_tex->Bind(1);
		material_data_cbuffer->UpdateData(devcon, &material.ads, sizeof(VertexTypes::PhongADS));
		instance_data_cbuffer->UpdateData(devcon, &transform.GetWorldMatrix(), sizeof(glm::mat4)); // not exactly safe yet
		mesh.mod->Bind();
		devcon->DrawIndexed(mesh.mod->GetNumTriangles() * 3, 0, 0);
	}

	Lighting::UnbindShadowDepthTexture(2);
#endif
}
void Renderer::RenderSkinned(Scene& scn)
{
#if GLACIER_OPENGL
	entt::registry& registry = scn.GetRegistry();

	auto curr_shader = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::TEXTURE_SKINNED_LIT);
	curr_shader->Bind();
	const GLint bone_matrices_uniform_loc = curr_shader->GetUniformLocation("bone_matrices[0]");
	const GLint world_matrix_uniform_loc = curr_shader->GetUniformLocation("world_matrix");
	const GLint material_ambient_uniform_loc = curr_shader->GetUniformLocation("material.ambient");
	const GLint material_diffuse_uniform_loc = material_ambient_uniform_loc + 1;
	const GLint material_specular_uniform_loc = material_diffuse_uniform_loc + 1;
	const GLint color_uniform_loc = curr_shader->GetUniformLocation("color");
	glUniform1iv(curr_shader->GetUniformLocation("textures[0]"), 4, std::array<GLint, 4>({ 0, 1, 2, 3 }).data());
	glUniform1i(curr_shader->GetUniformLocation("normal_map"), 4);
	glUniform1i(curr_shader->GetUniformLocation("dir_shadow_map"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, Lighting::Instance().DirShadow_tex);

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
#elif GLACIER_DIRECTX
	entt::registry& registry = scn.GetRegistry();

	// Get the DX variables
	auto devcon = DX::GetDeviceContext();
	auto instance_data_cbuffer = ShaderLoader::GetInstanceDataConstantBuffer();
	auto material_data_cbuffer = ShaderLoader::GetMaterialDataConstantBuffer();
	auto joint_data_cbuffer = ShaderLoader::GetJointDataConstantBuffer();

	auto curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::TEXTURE_SKINNED_LIT);
	curr_shader->Bind();

	// Render meshes with materials
	Lighting::BindShadowDepthTexture(2);
	//DX::DisableCulling();
	//DX::EnableBlending();
	auto render_group = registry.group<SkeletalMeshComponent>(entt::get<TransformComponent, MaterialComponent>);
	for (auto&& [entity, skel_mesh, transform, material] : render_group.each())
	{
		material.tex->Bind(0);
		material.norm_tex->Bind(1);
		material_data_cbuffer->UpdateData(devcon, &material.ads, sizeof(VertexTypes::PhongADS));
		joint_data_cbuffer->UpdateData(devcon, skel_mesh.bone_matrices, sizeof(VertexTypes::JointData));
		instance_data_cbuffer->UpdateData(devcon, &transform.GetWorldMatrix(), sizeof(glm::mat4));
		skel_mesh.mod->Bind();
		devcon->DrawIndexed(skel_mesh.mod->GetNumTriangles() * 3, 0, 0);
	}
	//DX::DisableBlending();
	//DX::EnableCulling();
	Lighting::UnbindShadowDepthTexture(2);
#endif
}
void Renderer::RenderUnlit(Scene& scn)
{
#if GLACIER_OPENGL
	// Render meshes without materials
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	auto shader = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::COLOR);
	shader->Bind();
	GLint world_matrix_uniform_loc = shader->GetUniformLocation("world_matrix");
	glUniform4fv(shader->GetUniformLocation("color"), 1, (const GLfloat*)&Colors::White);
	auto render_group = scn.GetRegistry().group(entt::get<MeshComponent, TransformComponent>, entt::exclude<MaterialComponent>);
	for (auto&& [entity, mesh, transform] : render_group.each())
	{
		glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&transform.GetWorldMatrix());
		glBindVertexArray(mesh.vao);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#elif GLACIER_DIRECTX
	UNREFERENCED_PARAMETER(scn);
#endif
}
void Renderer::RenderSkybox(Scene& scn)
{
	// Render skybox
#if GLACIER_OPENGL
	if (const SkyboxComponent* skybox = scn.GetFirstComponent<SkyboxComponent>())
	{
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_FRONT);

		auto curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::SKYBOX);
		curr_shader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->tex_id);
		auto skybox_model = (const ModelOpenGL*)ModelLoader::Get(PRELOADED_MODELS::UNIT_CUBE);
		glBindVertexArray(skybox_model->GetVAO());
		glDrawElements(GL_TRIANGLES, skybox_model->GetNumTriangles() * 3, GL_UNSIGNED_INT, nullptr);

		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);
	}
#elif GLACIER_DIRECTX
	if (const SkyboxComponent* skybox = scn.GetFirstComponent<SkyboxComponent>())
	{
		DX::EnableFrontFaceCulling();
		DX::SetDepthFunctionToLessEqual();

		auto curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::SKYBOX);
		curr_shader->Bind();
		
		auto skybox_model = ModelLoader::Get(PRELOADED_MODELS::UNIT_CUBE);
		skybox_model->Bind();
		
		skybox->tex->Bind(0);

		DX::GetDeviceContext()->DrawIndexed(skybox_model->GetNumTriangles() * 3, 0, 0);

		DX::SetDepthFunctionToLess();
		DX::EnableBackFaceCulling();
	}
#endif
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

#if GLACIER_OPENGL
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
#elif GLACIER_DIRECTX
	Lighting::RenderSceneShadows(&scn, camera);

	RenderLit(scn);
	RenderSkinned(scn);

	RenderSkybox(scn);
#endif
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