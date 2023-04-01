#include "gpch.h"
#include "TextureLoader.h"
#include "TextureAtt.h"

TextureLoader* TextureLoader::instance = nullptr;
const std::string TextureLoader::TEXTURE_PATH = "assets/textures/";

TextureLoader::TextureLoader()
	: textures()
{
	Texture* tex;

	tex = &preloaded_textures.emplace(PRELOADED_TEXTURES::DEFAULT, glm::vec4{ 1.0f, 0.0784f, 0.5764f, 1.0f }).first->second;
	TextureAtt::LoadGPUData(*tex);

	tex = &preloaded_textures.emplace(PRELOADED_TEXTURES::NORMAL_DEFAULT, glm::vec4{ 0.5f, 0.5f, 1.0f, 1.0f }).first->second;
	TextureAtt::LoadGPUData(*tex);
}

#if 1
Texture& TextureLoader::load_async_reg(const std::string& name, const std::string& file_name, const TextureParameters& tex_params)
{
	TextureOpenGL tex(file_name, tex_params);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	return textures.emplace(name, std::move(tex)).first->second;
}
Texture& TextureLoader::load_async_cube(const std::string& name, const std::array<std::string, 6>& file_paths, const TextureParameters& tex_params)
{
	TextureOpenGL cube_tex(file_paths, tex_params);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	return textures.emplace(name, std::move(cube_tex)).first->second;
}
void TextureLoader::load(const std::string& name, const std::string& file_name, const TextureParameters& tex_params)
{
	futures.push_back(std::async(std::launch::async, &TextureLoader::load_async_reg, this, name, TEXTURE_PATH + file_name, tex_params));
}
void TextureLoader::load(const std::string& name, const std::array<std::string, 6>& file_names, const TextureParameters& tex_params)
{
	const std::array<std::string, 6>& file_paths 
	{ 
		TEXTURE_PATH + file_names[0], 
		TEXTURE_PATH + file_names[1], 
		TEXTURE_PATH + file_names[2], 
		TEXTURE_PATH + file_names[3], 
		TEXTURE_PATH + file_names[4], 
		TEXTURE_PATH + file_names[5] 
	};
	futures.push_back(std::async(std::launch::async, &TextureLoader::load_async_cube, this, name, file_paths, tex_params));
}
void TextureLoader::load(const std::string& name, const glm::vec4& color)
{
	TextureOpenGL tex(color);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	Texture& ref = textures.emplace(name, color).first->second;
	TextureAtt::LoadGPUData(ref);
}
void TextureLoader::load(const std::string& name, const uint32_t width, const uint32_t height, const uint32_t num_channels, const uint8_t* data, const TextureParameters& tex_params)
{
	TextureOpenGL tex(width, height, num_channels, data, tex_params);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	Texture& ref = textures.emplace(name, std::move(tex)).first->second;
	TextureAtt::LoadGPUData(ref);
}

const TextureOpenGL& TextureLoader::get(const PRELOADED_TEXTURES preloaded_tex)
{
	auto it = preloaded_textures.find(preloaded_tex);
	assert(it != preloaded_textures.cend());
	return it->second;
}
const TextureOpenGL& TextureLoader::get(const std::string& name) const
{
	auto it = textures.find(name);
	assert(it != textures.cend());
	return it->second;
}
TextureOpenGL& TextureLoader::mod_get(const std::string& name)
{
	auto it = textures.find(name);
	assert(it != textures.end());
	return it->second;
}
#elif GLACIER_DIRECTX
Texture& TextureLoader::load_async_reg(const std::string& name, const std::string& file_name, const TextureParameters& tex_params)
{
	TextureDirectX tex(file_name, tex_params);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	return textures.emplace(name, std::move(tex)).first->second;
}
Texture& TextureLoader::load_async_cube(const std::string& name, const std::array<std::string, 6>& file_paths, const TextureParameters& tex_params)
{
	TextureDirectX cube_tex(file_paths, tex_params);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	return textures.emplace(name, std::move(cube_tex)).first->second;
}
void TextureLoader::load(const std::string& name, const std::string& file_name, const TextureParameters& tex_params)
{
	futures.push_back(std::async(std::launch::async, &TextureLoader::load_async_reg, this, name, TEXTURE_PATH + file_name, tex_params));
}
void TextureLoader::load(const std::string& name, const std::array<std::string, 6>& file_names, const TextureParameters& tex_params)
{
	const std::array<std::string, 6>& file_paths
	{
		TEXTURE_PATH + file_names[0],
		TEXTURE_PATH + file_names[1],
		TEXTURE_PATH + file_names[2],
		TEXTURE_PATH + file_names[3],
		TEXTURE_PATH + file_names[4],
		TEXTURE_PATH + file_names[5]
	};
	futures.push_back(std::async(std::launch::async, &TextureLoader::load_async_cube, this, name, file_paths, tex_params));
}
void TextureLoader::load(const std::string& name, const glm::vec4& color)
{
	TextureDirectX tex(color);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	Texture& ref = textures.emplace(name, color).first->second;
	TextureAtt::LoadGPUData(ref);
}
void TextureLoader::load(const std::string& name, const uint32_t width, const uint32_t height, const uint32_t num_channels, const uint8_t* data, const TextureParameters& tex_params)
{
	TextureDirectX tex(width, height, num_channels, data, tex_params);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	Texture& ref = textures.emplace(name, std::move(tex)).first->second;
	TextureAtt::LoadGPUData(ref);
}
const TextureDirectX& TextureLoader::get(const PRELOADED_TEXTURES preloaded_tex)
{
	auto it = preloaded_textures.find(preloaded_tex);
	assert(it != preloaded_textures.cend());
	return it->second;
}
const TextureDirectX& TextureLoader::get(const std::string& name) const
{
	auto it = textures.find(name);
	assert(it != textures.cend());
	return it->second;
}
TextureDirectX& TextureLoader::mod_get(const std::string& name)
{
	auto it = textures.find(name);
	assert(it != textures.end());
	return it->second;
}
#endif

void TextureLoader::WaitForThreadsAndLoadGPUData()
{
	TextureLoader& inst = Instance();
	while (!inst.futures.empty())
	{
		for (auto it = inst.futures.begin(); it != inst.futures.end(); it = std::next(it))
		{
			if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				TextureAtt::LoadGPUData(it->get());
				inst.futures.erase(it);
				break;
			}
		}
	}
}
void TextureLoader::Terminate()
{
	assert(instance && "TextureLoader instance wasn't created!");
	delete instance;
	instance = nullptr;
}