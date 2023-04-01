#ifndef _TEXTURE_LOADER
#define _TEXTURE_LOADER

#include "GlacierCore.h"
#include "TextureOpenGL.h"
#include "TextureDirectX.h"

enum class PRELOADED_TEXTURES
{
	DEFAULT,
	NORMAL_DEFAULT
};

class GLACIER_API TextureLoader
{
private:
	static const std::string TEXTURE_PATH;

	static TextureLoader* instance;
	static TextureLoader& Instance()
	{
		if (!instance)
			instance = new TextureLoader;
		return *instance;
	}
	TextureLoader();
	TextureLoader(const TextureLoader&) = delete;
	TextureLoader& operator=(const TextureLoader&) = delete;
	TextureLoader(TextureLoader&&) = delete;
	TextureLoader& operator=(TextureLoader&&) = delete;
	~TextureLoader() = default;

#if 1
	std::unordered_map<PRELOADED_TEXTURES, TextureOpenGL> preloaded_textures;
	std::unordered_map<std::string, TextureOpenGL> textures;
#elif GLACIER_DIRECTX
	std::unordered_map<PRELOADED_TEXTURES, TextureDirectX> preloaded_textures;
	std::unordered_map<std::string, TextureDirectX> textures;
#endif

	std::list<std::future<Texture&>> futures;
	std::mutex load_mutex;

	Texture& load_async_reg(const std::string& name, const std::string& file_path, const TextureParameters& tex_params);
	Texture& load_async_cube(const std::string& name, const std::array<std::string, 6>& file_paths, const TextureParameters& tex_params);
	void load(const std::string& name, const std::string& file_name, const TextureParameters& tex_params);
	void load(const std::string& name, const std::array<std::string, 6>& file_names, const TextureParameters& tex_params);
	void load(const std::string& name, const glm::vec4& color);
	void load(const std::string& name, const uint32_t width, const uint32_t height, const uint32_t num_channels, const uint8_t* data, const TextureParameters& tex_params);
	
#if 1
	const TextureOpenGL& get(const PRELOADED_TEXTURES preloaded_tex);
	const TextureOpenGL& get(const std::string& name) const;
	TextureOpenGL& mod_get(const std::string& name);
#elif GLACIER_DIRECTX
	const TextureDirectX& get(const PRELOADED_TEXTURES preloaded_tex);
	const TextureDirectX& get(const std::string& name) const;
	TextureDirectX& mod_get(const std::string& name);
#endif

	static void WaitForThreadsAndLoadGPUData();
	static void Terminate();

	friend class TextureLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name, const TextureParameters& tex_params = TextureParameters()) 
		{ Instance().load(name, file_name, tex_params); }
	static void Load(const std::string& name, const std::array<std::string, 6>& file_names, const TextureParameters& tex_params = 
			TextureParameters(TEXTURE_MIN_FILTER::LINEAR, TEXTURE_MAG_FILTER::LINEAR,
				TEXTURE_WRAP::CLAMP_TO_EDGE, TEXTURE_WRAP::CLAMP_TO_EDGE, TEXTURE_WRAP::CLAMP_TO_EDGE)) 
		{ Instance().load(name, file_names, tex_params); }
	static void Load(const std::string& name, const glm::vec4& color) 
		{ Instance().load(name, color); }
	static void Load(const std::string& name, const uint32_t width, const uint32_t height, const uint32_t num_channels, const uint8_t* data, const TextureParameters& tex_params = TextureParameters())
		{ Instance().load(name, width, height, num_channels, data, tex_params); }
	
#if 1
	static const TextureOpenGL& Get(const PRELOADED_TEXTURES preloaded_tex) 
		{ assert(instance && "TextureLoader not initialized!"); return instance->get(preloaded_tex); }
	static const TextureOpenGL& Get(const std::string& name)
		{ assert(instance && "TextureLoader not initialized!"); return instance->get(name); }
	static TextureOpenGL& ModGet(const std::string& name)
	{ assert(instance && "TextureLoader not initialized!"); return instance->mod_get(name); }
#elif GLACIER_DIRECTX
	static const TextureDirectX& Get(const PRELOADED_TEXTURES preloaded_tex)
		{ assert(instance && "TextureLoader not initialized!"); return instance->get(preloaded_tex); }
	static const TextureDirectX& Get(const std::string& name)
		{ assert(instance && "TextureLoader not initialized!"); return instance->get(name); }
	static TextureDirectX& ModGet(const std::string& name)
		{ assert(instance && "TextureLoader not initialized!"); return instance->mod_get(name); }
#endif
};

#endif _TEXTURE_LOADER