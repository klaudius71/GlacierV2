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

	Texture& load_async_reg(const std::string& name, const std::string& file_path);
	void load(const std::string& name, const std::string& file_name);
	Texture& load_async_cube(const std::string& name, const std::array<std::string, 6>& file_paths);
	void load(const std::string& name, const std::array<std::string, 6>& file_names);
	const Texture& get(const std::string& name);

	static void WaitForThreadsAndLoadGPUData();
	static void Terminate();

	friend class TextureLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name) { Instance().load(name, file_name); }
	static void Load(const std::string& name, const std::array<std::string, 6>& file_names) { Instance().load(name, file_names); }
	static const Texture& Get(const std::string& name) { return Instance().get(name); }
};

#endif _TEXTURE_LOADER