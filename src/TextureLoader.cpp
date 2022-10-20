#include "gpch.h"
#include "TextureLoader.h"

TextureLoader* TextureLoader::instance = nullptr;
const std::string TextureLoader::TEXTURE_PATH = "textures/";
std::list <std::future<Texture&>> TextureLoader::futures;
std::mutex TextureLoader::load_mutex;

Texture& TextureLoader::load_async_reg(const std::string& name, const std::string& file_name)
{
	Texture tex(file_name);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	return textures.emplace(name, std::move(tex)).first->second;
}
void TextureLoader::load(const std::string& name, const std::string& file_name)
{
	futures.push_back(std::async(std::launch::async, &TextureLoader::load_async_reg, this, name, TEXTURE_PATH + file_name));
}
Texture& TextureLoader::load_async_cube(const std::string& name, const std::array<std::string, 6>& file_paths)
{
	Texture cube_tex(file_paths);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(textures.find(name) == textures.end());
	return textures.emplace(name, std::move(cube_tex)).first->second;
}
void TextureLoader::load(const std::string& name, const std::array<std::string, 6>& file_names)
{
	const std::array<std::string, 6>& file_paths { TEXTURE_PATH + file_names[0], TEXTURE_PATH + file_names[1], TEXTURE_PATH + file_names[2], TEXTURE_PATH + file_names[3], TEXTURE_PATH + file_names[4], TEXTURE_PATH + file_names[5] };
	futures.push_back(std::async(std::launch::async, &TextureLoader::load_async_cube, this, name, file_paths));
}

const Texture& TextureLoader::get(const std::string& name)
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
				Texture::load_GPU_data(it->get());
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
