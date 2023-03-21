#ifndef _SHADER_LOADER
#define _SHADER_LOADER

#include "GlacierCore.h"
#include "Shader.h"

enum class PRELOADED_SHADERS
{
	COLOR,
	TEXTURE,
	TEXTURE_LIT,
	TEXTURE_SKINNED_LIT,
	SHADOW_MAP,
	SHADOW_MAP_SKINNED,
	SKYBOX,
	TEXT,
	TEXT_INSTANCED,
	SPRITE
};

class GLACIER_API ShaderLoader
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

	void load(const std::string& name, const std::string& file_name);
	void load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& fragment_shader_file_name);
	void load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& geometry_shader_file_name, const std::string& fragment_shader_file_name);

	Shader* const get(const PRELOADED_SHADERS shader);
	Shader* const get(const std::string& name);

	void load_matrix_binding(const Shader* shader);
	void load_light_bindings(const Shader* shader);
	void load_lightspace_bindings(const Shader* shader);

	static void Terminate();

	friend class ShaderLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name) { Instance().load(name, file_name); }
	static void Load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& fragment_shader_file_name) { Instance().load(name, vertex_shader_file_name, fragment_shader_file_name); }
	static void Load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& geometry_shader_file_name, const std::string& fragment_shader_file_name) { Instance().load(name, vertex_shader_file_name, geometry_shader_file_name, fragment_shader_file_name); }

	static Shader* Get(const PRELOADED_SHADERS shader) { return instance->get(shader); }
	static Shader* Get(const std::string& name) { return instance->get(name); }
	static const GLuint& GetMatricesUBO() { return instance->ubo_Matrices; }
	static const GLuint& GetDirLightUBO() { return instance->ubo_DirLight; }
	static const GLuint& GetLightspaceMatricesUBO() { return instance->ubo_LightspaceMatrices; }
};

#endif _SHADER_LOADER
