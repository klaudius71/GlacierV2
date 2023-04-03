#ifndef _SHADER_LOADER
#define _SHADER_LOADER

#include "GlacierCore.h"
#include "ShaderOpenGL.h"
#include "ShaderDirectX.h"

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

#if GLACIER_OPENGL
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
	
	std::unordered_map<PRELOADED_SHADERS, ShaderOpenGL> preloaded_shaders;
	std::unordered_map<std::string, ShaderOpenGL> shaders;
	GLuint ubo_Matrices;

	void load(const std::string& name, const std::string& file_name);
	void load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& fragment_shader_file_name);
	void load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& geometry_shader_file_name, const std::string& fragment_shader_file_name);

	Shader* const get(const PRELOADED_SHADERS shader);
	Shader* const get(const std::string& name);

	void load_matrix_binding(const ShaderOpenGL* shader);
	void load_light_bindings(const ShaderOpenGL* shader);
	void load_lightspace_bindings(const ShaderOpenGL* shader);

	static void Terminate();

	friend class ShaderLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name) { Instance().load(name, file_name); }
	static void Load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& fragment_shader_file_name) { Instance().load(name, vertex_shader_file_name, fragment_shader_file_name); }
	static void Load(const std::string& name, const std::string& vertex_shader_file_name, const std::string& geometry_shader_file_name, const std::string& fragment_shader_file_name) { Instance().load(name, vertex_shader_file_name, geometry_shader_file_name, fragment_shader_file_name); }

	static Shader* Get(const PRELOADED_SHADERS shader) { return instance->get(shader); }
	static Shader* Get(const std::string& name) { return instance->get(name); }
	static const GLuint GetMatricesUBO() { return instance->ubo_Matrices; }
};
#elif GLACIER_DIRECTX
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
	~ShaderLoader();

	std::unordered_map<PRELOADED_SHADERS, ShaderDirectX> preloaded_shaders;
	std::unordered_map<std::string, ShaderDirectX> shaders;

	ID3D11Buffer* camDataCBuffer;
	ID3D11Buffer* instanceDataCBuffer;
	ID3D11Buffer* spriteDataCBuffer;
	ID3D11Buffer* materialDataCBuffer;
	ID3D11Buffer* directionalLightCBuffer;

	void load(const std::string& name, const std::string& file_name);

	Shader* const get(const PRELOADED_SHADERS shader);
	Shader* const get(const std::string& name);

	static void Terminate();

	friend class ShaderLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name) { Instance().load(name, file_name); }

	static Shader* Get(const PRELOADED_SHADERS shader) { return instance->get(shader); }
	static Shader* Get(const std::string& name) { return instance->get(name); }

	static ID3D11Buffer* GetCamDataConstantBuffer() { return instance->camDataCBuffer; }
	static ID3D11Buffer* GetInstanceDataConstantBuffer() { return instance->instanceDataCBuffer; }
	static ID3D11Buffer* GetSpriteDataConstantBuffer() { return instance->spriteDataCBuffer;  }
	static ID3D11Buffer* GetMaterialDataConstantBuffer() { return instance->materialDataCBuffer; }
	static ID3D11Buffer* GetDirectionalLightConstantBuffer() { return instance->directionalLightCBuffer; }
};
#endif

#endif _SHADER_LOADER
