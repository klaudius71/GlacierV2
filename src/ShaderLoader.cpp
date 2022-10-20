#include "gpch.h"
#include "ShaderLoader.h"
#include "Shader.h"
#include "VertexTypes.h"
#include "Lighting.h"

ShaderLoader* ShaderLoader::instance = nullptr;
const std::string ShaderLoader::SHADER_PATH = "shaders/";

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

	// Load in the default shaders used by the engine
	auto curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::COLOR, SHADER_PATH + "color").first->second;
	load_matrix_binding(*curr_shader);
	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXTURE, SHADER_PATH + "texture").first->second;
	load_matrix_binding(*curr_shader);
	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXTURE_LIT, SHADER_PATH + "texture_lit").first->second;
	load_matrix_binding(*curr_shader);
	load_light_bindings(*curr_shader);
	load_lightspace_bindings(*curr_shader);
	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::SHADOW_MAP, SHADER_PATH + "shadow_map").first->second;
	load_lightspace_bindings(*curr_shader);
	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::SKYBOX, SHADER_PATH + "skybox").first->second;
	load_matrix_binding(*curr_shader);
	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXT, SHADER_PATH + "text").first->second;
	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::TEXT_INSTANCED, SHADER_PATH + "text_instanced").first->second;
	curr_shader = &preloaded_shaders.emplace(PRELOADED_SHADERS::SPRITE, SHADER_PATH + "sprite").first->second;
}

void ShaderLoader::load(const char* const name, const char* const file_name)
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

void ShaderLoader::load_matrix_binding(const Shader& shader)
{
	glUniformBlockBinding(shader.GetProgramID(), glGetUniformBlockIndex(shader.GetProgramID(), "Matrices"), 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_Matrices);
}
void ShaderLoader::load_light_bindings(const Shader& shader)
{
	glUniformBlockBinding(shader.GetProgramID(), glGetUniformBlockIndex(shader.GetProgramID(), "DirLight"), 1);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_DirLight);
}
void ShaderLoader::load_lightspace_bindings(const Shader& shader)
{
	glUniformBlockBinding(shader.GetProgramID(), glGetUniformBlockIndex(shader.GetProgramID(), "LightspaceMatrices"), 2);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo_LightspaceMatrices);
}

void ShaderLoader::Terminate()
{
	delete instance;
	instance = nullptr;
}