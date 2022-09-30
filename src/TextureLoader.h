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

	static std::vector<std::future<void>> futures;
	static std::mutex load_mutex;

	static void load_async(std::unordered_map<std::string, Texture>& list_ref, const std::string& name, const std::string& file_name);
	void load(const std::string& name, const std::string& file_name);
	const Texture& get(const std::string& name);

	static void WaitForThreadsAndLoadGPUData();
	static void Terminate();

	friend class TextureLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name) { Instance().load(name, file_name); }
	static const Texture& Get(const std::string& name) { return Instance().get(name); }
};

#endif _TEXTURE_LOADER