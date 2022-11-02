#ifndef _TEXTURE_LOADER
#define _TEXTURE_LOADER

#include "Texture.h"

class TextureLoader
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
	TextureLoader() = default;
	TextureLoader(const TextureLoader&) = delete;
	TextureLoader& operator=(const TextureLoader&) = delete;
	TextureLoader(TextureLoader&&) = delete;
	TextureLoader& operator=(TextureLoader&&) = delete;
	~TextureLoader() = default;

	std::unordered_map<std::string, Texture> textures;

	static std::list<std::future<Texture&>> futures;
	static std::mutex load_mutex;

	Texture& load_async_reg(const std::string& name, const std::string& file_path, const TextureParameters& tex_params);
	void load(const std::string& name, const std::string& file_name, const TextureParameters& tex_params);
	Texture& load_async_cube(const std::string& name, const std::array<std::string, 6>& file_paths, const TextureParameters& tex_params);
	void load(const std::string& name, const std::array<std::string, 6>& file_names, const TextureParameters& tex_params);
	void load(const std::string& name, const glm::vec4& color);
	
	Texture& get(const std::string& name);
	const Texture& get_const(const std::string& name) const;

	static void WaitForThreadsAndLoadGPUData();
	static void Terminate();

	friend class TextureLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name, const TextureParameters& tex_params = TextureParameters()) { Instance().load(name, file_name, tex_params); }
	static void Load(const std::string& name, const std::array<std::string, 6>& file_names, const TextureParameters& tex_params = 
		TextureParameters(TEXTURE_MIN_FILTER::LINEAR, TEXTURE_MAG_FILTER::LINEAR, 
			TEXTURE_WRAP::CLAMP_TO_EDGE, TEXTURE_WRAP::CLAMP_TO_EDGE, TEXTURE_WRAP::CLAMP_TO_EDGE)) 
	{ Instance().load(name, file_names, tex_params); }
	static void Load(const std::string& name, const glm::vec4& color) { Instance().load(name, color); }
	
	static Texture& Get(const std::string& name) { assert(instance && "TextureLoader not initialized!"); return instance->get(name); }
	static const Texture& GetConst(const std::string& name) { assert(instance && "TextureLoader not initialized!"); return instance->get_const(name); }
};

#endif _TEXTURE_LOADER