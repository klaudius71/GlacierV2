#include "gpch.h"
#include "ShaderLoader.h"
#include "VertexTypes.h"
#include "Lighting.h"
#include "DX.h"

ShaderLoader* ShaderLoader::instance = nullptr;

#if GLACIER_OPENGL
const std::string ShaderLoader::SHADER_PATH = "assets/shaders/";

ShaderLoader::ShaderLoader()
{
	// Set up the uniform buffer objects and their default values
	glGenBuffers(3, &ubo_Matrices);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_Matrices);
	const glm::mat4 idents[2] = { glm::mat4(1.0f), glm::mat4(1.0f) };
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, idents, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_DirLight);
	const VertexTypes::DirectionalLight dir_light(VertexTypes::PhongADS(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f), glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(VertexTypes::DirectionalLight), &dir_light, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_LightspaceMatrices);
	const glm::mat4 ident(1.0f);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &ident, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Lighting::SetBuffers(ubo_DirLight, ubo_LightspaceMatrices);

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
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_Matrices);
}
void ShaderLoader::load_light_bindings(const ShaderOpenGL* shader)
{
	glUniformBlockBinding(shader->GetProgramID(), glGetUniformBlockIndex(shader->GetProgramID(), "DirLight"), 1);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_DirLight);
}
void ShaderLoader::load_lightspace_bindings(const ShaderOpenGL* shader)
{
	glUniformBlockBinding(shader->GetProgramID(), glGetUniformBlockIndex(shader->GetProgramID(), "LightspaceMatrices"), 2);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo_LightspaceMatrices);
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
	// --Load in the default shaders used by the engine--
	preloaded_shaders.emplace(PRELOADED_SHADERS::TEXTURE, SHADER_PATH + "Texture.hlsl");
	preloaded_shaders.emplace(PRELOADED_SHADERS::SPRITE, SHADER_PATH + "Sprite.hlsl");
	preloaded_shaders.emplace(PRELOADED_SHADERS::TEXT, SHADER_PATH + "Text.hlsl");

	auto dev = DX::GetDevice();

	// Create the constant buffers
	HRESULT hr;
	// View Projection buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexTypes::CamData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = dev->CreateBuffer(&bd, nullptr, &camDataCBuffer);
	assert(SUCCEEDED(hr));

	bd.ByteWidth = sizeof(VertexTypes::InstanceData);
	hr = dev->CreateBuffer(&bd, nullptr, &instanceDataCBuffer);
	assert(SUCCEEDED(hr));

	bd.ByteWidth = sizeof(VertexTypes::SpriteData);
	hr = dev->CreateBuffer(&bd, nullptr, &spriteDataCBuffer);
	assert(SUCCEEDED(hr));
	
	// Set the common constant buffers to context
	auto context = DX::GetDeviceContext();
	context->VSSetConstantBuffers(0, 1, &camDataCBuffer);
	context->VSSetConstantBuffers(1, 1, &instanceDataCBuffer);

	context->PSSetConstantBuffers(0, 1, &camDataCBuffer);
	context->PSSetConstantBuffers(1, 1, &instanceDataCBuffer);
}
ShaderLoader::~ShaderLoader()
{
	camDataCBuffer->Release();
	instanceDataCBuffer->Release();
	spriteDataCBuffer->Release();
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