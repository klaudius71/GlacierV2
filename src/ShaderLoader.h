#ifndef _SHADER_LOADER
#define _SHADER_LOADER

#include "gpch.h"

class Shader;

enum class PRELOADED_SHADERS
{
	COLOR,
	TEXTURE,
	TEXTURE_LIT,
	SHADOW_MAP,
	SKYBOX,
	TEXT,
	TEXT_INSTANCED,
	SPRITE
};

class ShaderLoader
{
private:
	static const std::string SHADER_PATH;

	static ShaderLoader* instance;
	static ShaderLoader& Instance()
	{
		if (!instance)
			instance = new ShaderLoader;
		return *instance;
	}
	ShaderLoader();
	ShaderLoader(const ShaderLoader&) = delete;
	ShaderLoader& operator=(const ShaderLoader&) = delete;
	ShaderLoader(ShaderLoader&&) = delete;
	ShaderLoader& operator=(ShaderLoader&&) = delete;
	~ShaderLoader() = default;
	
	std::unordered_map<PRELOADED_SHADERS, Shader> preloaded_shaders;
	std::unordered_map<std::string, Shader> shaders;
	GLuint ubo_Matrices;
	GLuint ubo_DirLight;
	GLuint ubo_LightspaceMatrices;

	void load(const char* const name, const char* const file_name);
	Shader* const get(const PRELOADED_SHADERS shader);
	Shader* const get(const std::string& name);

	void load_matrix_binding(const Shader& shader);
	void load_light_bindings(const Shader& shader);
	void load_lightspace_bindings(const Shader& shader);

	static void Terminate();

	friend class ShaderLoaderAtt;

public:
	static void Load(const char* const name, const char* const file_name) { Instance().load(name, file_name); }

	static Shader* Get(const PRELOADED_SHADERS shader) { return instance->get(shader); }
	static Shader* Get(const std::string& name) { return instance->get(name); }
	static const GLuint& GetMatricesUBO() { return instance->ubo_Matrices; }
	static const GLuint& GetDirLightUBO() { return instance->ubo_DirLight; }
	static const GLuint& GetLightspaceMatricesUBO() { return instance->ubo_LightspaceMatrices; }
};

#endif _SHADER_LOADER
