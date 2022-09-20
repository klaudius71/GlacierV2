#include "gpch.h"
#include "TextureLoader.h"

TextureLoader* TextureLoader::instance = nullptr;
const std::string TextureLoader::TEXTURE_PATH = "textures/";
std::vector<std::future<void>> TextureLoader::futures;
std::mutex TextureLoader::load_mutex;

void TextureLoader::load_async(std::unordered_map<std::string, Texture>& list_ref, const std::string& name, const std::string& file_name)
{
	Texture tex(file_name);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(list_ref.find(name) == list_ref.end());
	list_ref.emplace(name, std::move(tex));
}
void TextureLoader::load(const std::string& name, const std::string& file_name)
{
	futures.push_back(std::async(std::launch::async, load_async, std::ref(textures), name, TEXTURE_PATH + file_name));
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
		futures.pop_back();

	for (auto& model : instance->textures)
		Texture::load_GPU_data(model.second);
}
void TextureLoader::Terminate()
{
	delete instance;
	instance = nullptr;
}
