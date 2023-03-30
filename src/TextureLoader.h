#ifndef _TEXTURE_LOADER
#define _TEXTURE_LOADER

#include "GlacierCore.h"
#include "Texture.h"

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

	std::unordered_map<PRELOADED_TEXTURES, Texture> preloaded_textures;
	std::unordered_map<std::string, Texture> textures;

	static std::list<std::future<Texture&>> futures;
	static std::mutex load_mutex;

	Texture& load_async_reg(const std::string& name, const std::string& file_path, const TextureParameters& tex_params);
	void load(const std::string& name, const std::string& file_name, const TextureParameters& tex_params);
	Texture& load_async_cube(const std::string& name, const std::array<std::string, 6>& file_paths, const TextureParameters& tex_params);
	void load(const std::string& name, const std::array<std::string, 6>& file_names, const TextureParameters& tex_params);
	void load(const std::string& name, const glm::vec4& color);
	void load(const std::string& name, const uint32_t width, const uint32_t height, const uint32_t num_channels, const uint8_t* data, const TextureParameters& tex_params);
	
	const Texture& get(const PRELOADED_TEXTURES preloaded_tex);
	const Texture& get(const std::string& name) const;
	Texture& mod_get(const std::string& name);

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
	
	static const Texture& Get(const PRELOADED_TEXTURES preloaded_tex) 
		{ assert(instance && "TextureLoader not initialized!"); return instance->get(preloaded_tex); }
	static const Texture& Get(const std::string& name)
		{ assert(instance && "TextureLoader not initialized!"); return instance->get(name); }
	static Texture& ModGet(const std::string& name)
	{ assert(instance && "TextureLoader not initialized!"); return instance->mod_get(name); }
};

#endif _TEXTURE_LOADER