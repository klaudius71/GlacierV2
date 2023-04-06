#include "gpch.h"
#include "ShaderLoader.h"
#include "VertexTypes.h"
#include "Lighting.h"
#include "DX.h"
#include "ConstantBuffer.h"

ShaderLoader* ShaderLoader::instance = nullptr;

#if GLACIER_OPENGL
const std::string ShaderLoader::SHADER_PATH = "assets/shaders/";

ShaderLoader::ShaderLoader()
{
	glGenBuffers(1, &ubo_Matrices);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_Matrices);
	const glm::mat4 idents[2] = { glm::mat4(1.0f), glm::mat4(1.0f) };
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, idents, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_Matrices);

	// --Load in the default shaders used by the engine--
	auto curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::COLOR, SHADER_PATH + "color").first->second;
	load_matrix_binding(curr_shader);

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXTURE, SHADER_PATH + "texture").first->second;
	load_matrix_binding(curr_shader);

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXTURE_LIT, SHADER_PATH + "texture_lit").first->second;
	load_matrix_binding(curr_shader);
	load_light_bindings(curr_shader);
	load_lightspace_bindings(curr_shader);

	curr_shader = &preloaded_shaders.emplace(std::piecewise_construct, 
											std::forward_as_tuple(PRELOADED_SHADERS::TEXTURE_SKINNED_LIT), 
											std::forward_as_tuple(SHADER_PATH + "texture_skinned_lit.vs.glsl", SHADER_PATH + "texture_lit.fs.glsl")).first->second;
	load_matrix_binding(curr_shader);
	load_light_bindings(curr_shader);
	load_lightspace_bindings(curr_shader);

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::SHADOW_MAP, SHADER_PATH + "shadow_map").first->second;
	load_lightspace_bindings(curr_shader);

	curr_shader = &preloaded_shaders.emplace(std::piecewise_construct, 
											std::forward_as_tuple(PRELOADED_SHADERS::SHADOW_MAP_SKINNED), 
											std::forward_as_tuple(SHADER_PATH + "shadow_map_skinned.vs.glsl", SHADER_PATH + "shadow_map.fs.glsl")).first->second;
	load_lightspace_bindings(curr_shader);

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::SKYBOX, SHADER_PATH + "skybox").first->second;
	load_matrix_binding(curr_shader);

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXT, SHADER_PATH + "text").first->second;
	
	curr_shader = &preloaded_shaders.emplace(std::piecewise_construct, 
											std::forward_as_tuple(PRELOADED_SHADERS::TEXT_INSTANCED), 
											std::forward_as_tuple(SHADER_PATH + "text_instanced.vs.glsl", SHADER_PATH + "text.fs.glsl")).first->second;

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::SPRITE, SHADER_PATH + "sprite").first->second;
}

void ShaderLoader::load(const std::string& name, const std::string& file_name)
{
	assert(shaders.find(name) != shaders.cend() && "Attempted to create a duplicate shader!");
	shaders.emplace(name, SHADER_PATH + file_name);
}
void ShaderLoader::load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& fragment_shader_file_name)
{
	assert(shaders.find(name) != shaders.cend() && "Attempted to create a duplicate shader!");
	shaders.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(SHADER_PATH + vertex_shader_file_name, SHADER_PATH + fragment_shader_file_name));
}
void ShaderLoader::load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& geometry_shader_file_name, const std::string& fragment_shader_file_name)
{
	assert(shaders.find(name) != shaders.cend() && "Attempted to create a duplicate shader!");
	shaders.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(SHADER_PATH + vertex_shader_file_name, SHADER_PATH + geometry_shader_file_name, SHADER_PATH + fragment_shader_file_name));
}

Shader* const ShaderLoader::get(const PRELOADED_SHADERS shader)
{
	return &preloaded_shaders.at(shader);
}
Shader* const ShaderLoader::get(const std::string& name)
{
	const auto it = shaders.find(name);
	assert(it != shaders.cend() && "Shader not found!");
	return &it->second;
}

void ShaderLoader::load_matrix_binding(const ShaderOpenGL* shader)
{
	glUniformBlockBinding(shader->GetProgramID(), glGetUniformBlockIndex(shader->GetProgramID(), "Matrices"), 0);
}
void ShaderLoader::load_light_bindings(const ShaderOpenGL* shader)
{
	glUniformBlockBinding(shader->GetProgramID(), glGetUniformBlockIndex(shader->GetProgramID(), "DirLight"), 1);
}
void ShaderLoader::load_lightspace_bindings(const ShaderOpenGL* shader)
{
	glUniformBlockBinding(shader->GetProgramID(), glGetUniformBlockIndex(shader->GetProgramID(), "LightspaceMatrices"), 2);
}

void ShaderLoader::Terminate()
{
	delete instance;
	instance = nullptr;
}
#elif GLACIER_DIRECTX
const std::string ShaderLoader::SHADER_PATH = "assets/shaders/dx11/";

ShaderLoader::ShaderLoader()
{
	// Create the constant buffers
	camDataCBuffer = new ConstantBuffer(sizeof(VertexTypes::CamData));
	instanceDataCBuffer = new ConstantBuffer(sizeof(VertexTypes::InstanceData));
	spriteDataCBuffer = new ConstantBuffer(sizeof(VertexTypes::SpriteData));
	materialDataCBuffer = new ConstantBuffer(sizeof(VertexTypes::PhongADS));
	directionalLightCBuffer = new ConstantBuffer(sizeof(VertexTypes::DirectionalLight));
	
	// --Load in the default shaders used by the engine--
	auto curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXTURE, SHADER_PATH + "Texture.hlsl").first->second;
	curr_shader->AddConstantBuffer(camDataCBuffer, 0);
	curr_shader->AddConstantBuffer(instanceDataCBuffer, 1);

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXTURE_LIT, SHADER_PATH + "TextureLit.hlsl").first->second;
	curr_shader->AddConstantBuffer(camDataCBuffer, 0);
	curr_shader->AddConstantBuffer(instanceDataCBuffer, 1);
	curr_shader->AddConstantBuffer(materialDataCBuffer, 2);
	curr_shader->AddConstantBuffer(directionalLightCBuffer, 3);

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::SKYBOX, SHADER_PATH + "Skybox.hlsl").first->second;
	curr_shader->AddConstantBuffer(camDataCBuffer, 0);

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::SPRITE, SHADER_PATH + "Sprite.hlsl").first->second;
	curr_shader->AddConstantBuffer(camDataCBuffer, 0);
	curr_shader->AddConstantBuffer(instanceDataCBuffer, 1);
	curr_shader->AddConstantBuffer(spriteDataCBuffer, 2);

	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXT, SHADER_PATH + "Text.hlsl").first->second;
	curr_shader->AddConstantBuffer(camDataCBuffer, 0);
	curr_shader->AddConstantBuffer(instanceDataCBuffer, 1);
	curr_shader->AddConstantBuffer(spriteDataCBuffer, 2);

	auto dev = DX::GetDevice();

	// Set the common constant buffers to context
	camDataCBuffer->Bind(0);
	instanceDataCBuffer->Bind(1);
}
ShaderLoader::~ShaderLoader()
{
	delete camDataCBuffer;
	delete instanceDataCBuffer;
	delete spriteDataCBuffer;
	delete materialDataCBuffer;
	delete directionalLightCBuffer;
}

void ShaderLoader::load(const std::string& name, const std::string& file_name)
{
	assert(shaders.find(name) != shaders.cend() && "Attempted to create a duplicate shader!");
	shaders.emplace(name, SHADER_PATH + file_name);
}

Shader* const ShaderLoader::get(const PRELOADED_SHADERS shader)
{
	return &preloaded_shaders.at(shader);
}
Shader* const ShaderLoader::get(const std::string& name)
{
	const auto it = shaders.find(name);
	assert(it != shaders.cend() && "Shader not found!");
	return &it->second;
}

void ShaderLoader::Terminate()
{
	delete instance;
	instance = nullptr;
}
#endif