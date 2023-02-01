#include "gpch.h"
#include "ModelLoader.h"
#include "Model.h"
#include "ModelAtt.h"

ModelLoader* ModelLoader::instance = nullptr;
const std::string ModelLoader::MODEL_PATH = "models/";
std::vector<std::future<Model&>> ModelLoader::futures;
std::mutex ModelLoader::load_mutex;

ModelLoader::ModelLoader()
{
	std::vector<VertexTypes::Vertex> verts;
	std::vector<VertexTypes::VertexTriangle> tris;
	verts.emplace_back(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	verts.emplace_back(-1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
	verts.emplace_back( 1.0f,  1.0f, 0.0f, 1.0f, 1.0f);
	verts.emplace_back( 1.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	tris.emplace_back(0, 2, 1);
	tris.emplace_back(0, 3, 2);
	preloaded_models.emplace(std::piecewise_construct, std::forward_as_tuple(PRELOADED_MODELS::QUAD), std::forward_as_tuple(verts, tris));
	verts.clear();
	tris.clear();

	Model& skybox_mod = preloaded_models.emplace(std::piecewise_construct, std::forward_as_tuple(PRELOADED_MODELS::UNIT_CUBE), std::forward_as_tuple(PREMADE_MODELS::UNIT_CUBE_REPEAT_TEXTURE, 1.0f)).first->second;
	ModelAtt::LoadGPUData(skybox_mod);
}

Model& ModelLoader::load_async_file2(const std::string& name, const std::string& file_name, const bool& glacier)
{
	GLACIER_DEBUG_FUNC_TIMER("Loaded " + file_name + " in... ");
	Model mod(MODEL_PATH + file_name, glacier);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicate model!");
	return models.emplace(name, std::move(mod)).first->second;
}
void ModelLoader::load(const std::string& name, const std::string& file_name, const bool& glacier)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_file2, this, name, file_name, glacier));
}
Model& ModelLoader::load_async_file(const std::string& name, const std::string& file_name)
{
	GLACIER_DEBUG_FUNC_TIMER("Loaded " + file_name + " in... ");
	Model mod(MODEL_PATH + file_name);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicate model!");
	return models.emplace(name, std::move(mod)).first->second;
}
void ModelLoader::load(const std::string& name, const std::string& file_name)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_file, this, name, file_name));
}
Model& ModelLoader::load_async_pre(const std::string& name, PREMADE_MODELS premade_model, const float& scale)
{
	Model mod(premade_model, scale);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicate model!");
	return models.emplace(name, std::move(mod)).first->second;
}
void ModelLoader::load(const std::string& name, PREMADE_MODELS premade_model, const float& scale)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_pre, this, name, premade_model, scale));
}
Model& ModelLoader::load_async_hgtmap(const std::string& name, const std::string& file_name, const float& xz_size, const float& max_height, const float& u, const float& v)
{
	Model mod(file_name, xz_size, max_height, u, v);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicated model!");
	return models.emplace(name, std::move(mod)).first->second;
}
void ModelLoader::load(const std::string& name, const std::string& file_name, const float& xz_size, const float& max_height, const float& u, const float& v)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_hgtmap, this, name, file_name, xz_size, max_height, u, v));
}
Model& ModelLoader::load_async_plane(const std::string& name, const float& xz_size, const float& u, const float& v)
{
	Model mod(xz_size, u, v);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicated model!");
	return models.emplace(name, std::move(mod)).first->second;
}
void ModelLoader::load(const std::string& name, const float& xz_size, const float& u, const float& v)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_plane, this, name, xz_size, u, v));
}
Model& ModelLoader::load_async_sphere(const std::string& name, const uint32_t& v_slices, const uint32_t& h_slices)
{
	Model mod(v_slices, h_slices);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicated model!");
	return models.emplace(name, std::move(mod)).first->second;
}
void ModelLoader::load(const std::string& name, const uint32_t& v_slices, const uint32_t& h_slices)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_sphere, this, name, v_slices, h_slices));
}

Model* const ModelLoader::get(const PRELOADED_MODELS model)
{
	return &preloaded_models.at(model);
}
Model* const ModelLoader::get(const std::string& name)
{
	const auto it = models.find(name);
	assert(it != models.cend() && "Model not found!");
	return &it->second;
}

void ModelLoader::WaitForThreadsAndLoadGPUData()
{
	while (!futures.empty())
	{
		for (auto v = futures.begin(); v != futures.end(); v++)
		{
			if (v->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				ModelAtt::LoadGPUData(v->get());
				futures.erase(v);
				break;
			}
		}
	}
}
void ModelLoader::Terminate()
{
	delete instance;
	instance = nullptr;
}