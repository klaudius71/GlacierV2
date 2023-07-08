#include "gpch.h"
#include "Lighting.h"
#include "Scene.h"
#include "Components.h"
#include "ShaderLoader.h"
#include "ShaderOpenGL.h"
#include "Application.h"
#include "Window.h"
#include "ModelOpenGL.h"
#include "DX.h"
#include "ConstantBuffer.h"

Lighting* Lighting::instance = nullptr;
const DirectionalLightComponent Lighting::default_dir_light = DirectionalLightComponent(VertexTypes::PhongADS(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f), glm::vec3(0.0f));

#if GLACIER_OPENGL
Lighting::Lighting()
	: DirLight_ubo(0), LightspaceMatrices_ubo(0), DirShadow_fbo(0), DirShadow_tex(0)
{
	// Set up the uniform buffer objects and their default values
	glGenBuffers(2, &DirLight_ubo);

	glBindBuffer(GL_UNIFORM_BUFFER, DirLight_ubo);
	const VertexTypes::DirectionalLight dir_light(VertexTypes::PhongADS(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f), glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(VertexTypes::DirectionalLight), &dir_light, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, DirLight_ubo);

	glBindBuffer(GL_UNIFORM_BUFFER, LightspaceMatrices_ubo);
	const glm::mat4 ident(1.0f);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &ident, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, LightspaceMatrices_ubo);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Generate the framebuffer for shadow mapping
	glGenFramebuffers(1, &DirShadow_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, DirShadow_fbo);

	glGenTextures(1, &DirShadow_tex);
	glBindTexture(GL_TEXTURE_2D, DirShadow_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DIR_SHADOW_MAP_SIZE, DIR_SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&Colors::White);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DirShadow_tex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
Lighting::~Lighting()
{
	glDeleteBuffers(2, &DirLight_ubo);
	glDeleteFramebuffers(1, &DirShadow_fbo);
	glDeleteTextures(1, &DirShadow_tex);
}

void Lighting::updateBuffers(const Scene& curr_scene)
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

void Lighting::renderSceneShadows(Scene* const curr_scene, const CameraComponent& cam)
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
	const glm::mat4 lightspace = glm::ortho(-512.0f, 512.0f, -512.0f, 512.0f, -1024.0f, 1024.0f) *
		glm::lookAt(dir_light_cam_center_pos - dir_light_dir * 128.0f, dir_light_cam_center_pos, glm::vec3(0.0f, 1.0f, 0.0f));

	glBindBuffer(GL_UNIFORM_BUFFER, LightspaceMatrices_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &lightspace);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	auto curr_shader = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::SHADOW_MAP);
	curr_shader->Bind();

	GLint world_matrix_uniform_loc = curr_shader->GetUniformLocation("world_matrix");
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

	curr_shader = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::SHADOW_MAP_SKINNED);
	curr_shader->Bind();

	world_matrix_uniform_loc = curr_shader->GetUniformLocation("world_matrix");
	GLint bone_matrices_uniform_loc = curr_shader->GetUniformLocation("bone_matrices[0]");
	auto skel_mesh_anim_group = scene_registry.group<SkeletalMeshComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, skel_mesh, transform] : skel_mesh_anim_group.each())
	{
		if (skel_mesh.cast_shadow)
		{
			glBindVertexArray(skel_mesh.mod->GetVAO());
			glUniformMatrix4fv(bone_matrices_uniform_loc, skel_mesh.num_joints, GL_FALSE, (const GLfloat*)skel_mesh.bone_matrices);
			glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&transform.GetWorldMatrix());
			glDrawElements(GL_TRIANGLES, skel_mesh.num_indices, GL_UNSIGNED_INT, nullptr);
		}
	}

	//glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Glacier::GetWindow().GetWindowWidth(), Glacier::GetWindow().GetWindowHeight());

	/*ImGui::Begin("Debug");
	ImGui::Text("DirLight Shadow Texture:");
	ImGui::Image((ImTextureID)(uint64_t)DirShadow_tex, ImVec2{256, 256}, ImVec2{0,0}, ImVec2{1,1});
	ImGui::End();*/
}
#elif GLACIER_DIRECTX
Lighting::Lighting()
	: directionalLightCBuffer(new ConstantBuffer(sizeof(VertexTypes::DirectionalLight))),
	lightspaceMatrixCBuffer(new ConstantBuffer(sizeof(VertexTypes::LightspaceData))),
	shadowDepthStencilView(nullptr),
	shadowShaderResourceView(nullptr),
	shadowSamplerState(nullptr)
{
	HRESULT hr;
	ID3D11Device* dev = DX::GetDevice();

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = DIR_SHADOW_MAP_SIZE;
	descDepth.Height = DIR_SHADOW_MAP_SIZE;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality |= D3D11_CENTER_MULTISAMPLE_PATTERN;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* tex = nullptr;
	hr = dev->CreateTexture2D(&descDepth, nullptr, &tex);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = dev->CreateDepthStencilView(tex, &descDSV, &shadowDepthStencilView);
	assert(SUCCEEDED(hr));

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels = 1;

	hr = dev->CreateShaderResourceView(tex, &viewDesc, &shadowShaderResourceView);
	assert(SUCCEEDED(hr));

	tex->Release();

	D3D11_SAMPLER_DESC samp;
	ZeroMemory(&samp, sizeof(D3D11_SAMPLER_DESC));
	samp.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
	samp.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samp.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samp.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samp.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samp.MinLOD = 0;
	samp.MaxLOD = D3D11_FLOAT32_MAX;
	samp.BorderColor[0] = 1.0f;
	samp.BorderColor[1] = 1.0f;
	samp.BorderColor[2] = 1.0f;
	samp.BorderColor[3] = 1.0f;
	hr = dev->CreateSamplerState(&samp, &shadowSamplerState);
	assert(SUCCEEDED(hr));
}
Lighting::~Lighting()
{
	delete directionalLightCBuffer;
	delete lightspaceMatrixCBuffer;

	shadowDepthStencilView->Release();
	shadowShaderResourceView->Release();
	shadowSamplerState->Release();
}

void Lighting::bindShadowDepthTexture(const UINT index)
{
	auto devcon = DX::GetDeviceContext();
	devcon->PSSetShaderResources(index, 1, &shadowShaderResourceView);
	devcon->PSSetSamplers(index, 1, &shadowSamplerState);
}
void Lighting::unbindShadowDepthTexture(const UINT index)
{
	ID3D11ShaderResourceView* hm[]{ nullptr }; // -_-
	DX::GetDeviceContext()->PSSetShaderResources(index, 1, hm);
}

void Lighting::updateBuffers(const Scene& curr_scene)
{
	auto devcon = DX::GetDeviceContext();
	if (const DirectionalLightComponent* dir_light = curr_scene.GetFirstComponent<DirectionalLightComponent>())
		directionalLightCBuffer->UpdateData(devcon, &dir_light->light, sizeof(VertexTypes::DirectionalLight));
	else
		directionalLightCBuffer->UpdateData(devcon, &default_dir_light.light, sizeof(VertexTypes::DirectionalLight));
}

void Lighting::renderSceneShadows(Scene* const curr_scene, const CameraComponent& cam)
{
	entt::registry& scene_registry = curr_scene->GetRegistry();

	auto dir_light = curr_scene->GetFirstComponent<DirectionalLightComponent>();
	if (!dir_light)
		return;

	auto devcon = DX::GetDeviceContext();
	auto instanceDataCBuffer = ShaderLoader::GetInstanceDataConstantBuffer();
	auto jointDataCBuffer = ShaderLoader::GetJointDataConstantBuffer();

	DX::SetViewport(0, 0, DIR_SHADOW_MAP_SIZE, DIR_SHADOW_MAP_SIZE);
	devcon->ClearDepthStencilView(shadowDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, NULL);
	devcon->OMSetRenderTargets(0, nullptr, shadowDepthStencilView);
	DX::EnableFrontFaceCulling();

	auto& dir_light_dir = dir_light->light.direction;
	const glm::vec3 dir_light_cam_center_pos = cam.cam_pos + glm::normalize(cam.cam_dir) * 256.0f;
	const glm::mat4 lightspace = glm::ortho(-512.0f, 512.0f, -512.0f, 512.0f, -1024.0f, 1024.0f) *
								glm::lookAt(dir_light_cam_center_pos - dir_light_dir * 128.0f, dir_light_cam_center_pos, glm::vec3(0.0f, 1.0f, 0.0f));

	lightspaceMatrixCBuffer->UpdateData(devcon, &lightspace, sizeof(glm::mat4));

	auto curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::SHADOW_MAP);
	curr_shader->Bind();
	devcon->PSSetShader(nullptr, nullptr, 0);

	auto mesh_transform_group = scene_registry.group<MeshComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, mesh, transform] : mesh_transform_group.each())
	{
		if (mesh.cast_shadow)
		{
			mesh.mod->Bind();
			instanceDataCBuffer->UpdateData(devcon, &transform.GetWorldMatrix(), sizeof(glm::mat4));
			devcon->DrawIndexed(mesh.mod->GetNumTriangles() * 3, 0, 0);
		}
	}

	curr_shader = ShaderLoader::Get(PRELOADED_SHADERS::SHADOW_MAP_SKINNED);
	curr_shader->Bind();
	devcon->PSSetShader(nullptr, nullptr, 0);
	
	DX::DisableCulling();
	auto skel_mesh_anim_group = scene_registry.group<SkeletalMeshComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, skel_mesh, transform] : skel_mesh_anim_group.each())
	{
		if (skel_mesh.cast_shadow)
		{
			skel_mesh.mod->Bind();
			jointDataCBuffer->UpdateData(devcon, skel_mesh.bone_matrices, sizeof(VertexTypes::JointData));
			instanceDataCBuffer->UpdateData(devcon, &transform.GetWorldMatrix(), sizeof(glm::mat4));
			devcon->DrawIndexed(skel_mesh.mod->GetNumTriangles() * 3, 0, 0);
		}
	}

	DX::EnableBackFaceCulling();
	DX::ResetRenderTarget();
	DX::SetViewport(0, 0, (float)Glacier::GetWindow().GetWindowWidth(), (float)Glacier::GetWindow().GetWindowHeight());
}
#endif

void Lighting::Initialize()
{
	assert(!instance && "Lighting already initialized!");
	instance = new Lighting;
}
void Lighting::Terminate()
{
	assert(instance && "Lighting not initialized!");
	delete instance;
}