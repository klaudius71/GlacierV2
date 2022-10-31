#include "gpch.h"
#include "TextureLoader.h"
#include "TextureAtt.h"

TextureLoader* TextureLoader::instance = nullptr;
const std::string TextureLoader::TEXTURE_PATH = "textures/";
std::list <std::future<Texture&>> TextureLoader::futures;
std::mutex TextureLoader::load_mutex;

Texture& TextureLoader::load_async_reg(const std::string& name, const std::string& file_name, const TextureParameters& tex_params)
{
	Texture tex(file_name, tex_params);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	return textures.emplace(name, std::move(tex)).first->second;
}
void TextureLoader::load(const std::string& name, const std::string& file_name, const TextureParameters& tex_params)
{
	futures.push_back(std::async(std::launch::async, &TextureLoader::load_async_reg, this, name, TEXTURE_PATH + file_name, tex_params));
}
Texture& TextureLoader::load_async_cube(const std::string& name, const std::array<std::string, 6>& file_paths, const TextureParameters& tex_params)
{
	Texture cube_tex(file_paths, tex_params);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	return textures.emplace(name, std::move(cube_tex)).first->second;
}
void TextureLoader::load(const std::string& name, const std::array<std::string, 6>& file_names, const TextureParameters& tex_params)
{
	const std::array<std::string, 6>& file_paths { TEXTURE_PATH + file_names[0], TEXTURE_PATH + file_names[1], TEXTURE_PATH + file_names[2], TEXTURE_PATH + file_names[3], TEXTURE_PATH + file_names[4], TEXTURE_PATH + file_names[5] };
	//futures.push_back(std::async(std::launch::async, &TextureLoader::load_async_cube, this, name, file_paths));
	load_async_cube(name, file_paths, tex_params);
}

Texture& TextureLoader::get(const std::string& name)
{
	auto it = textures.find(name);
	assert(it != textures.cend());
	return it->second;
}
const Texture& TextureLoader::get_const(const std::string& name) const
{
	auto it = textures.find(name);
	assert(it != textures.cend());
	return it->second;
}

void TextureLoader::WaitForThreadsAndLoadGPUData()
{
	while (!futures.empty())
	{
		for (auto it = futures.begin(); it != futures.end(); it = std::next(it))
		{
			if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				TextureAtt::LoadGPUData(it->get());
				futures.erase(it);
				break;
			}
		}
	}
}
void TextureLoader::Terminate()
{
	delete instance;
	instance = nullptr;
}