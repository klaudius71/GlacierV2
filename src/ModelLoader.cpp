#include "gpch.h"
#include "ModelLoader.h"
#include "Model.h"

ModelLoader* ModelLoader::instance = nullptr;
const std::string ModelLoader::MODEL_PATH = "models/";
std::vector<std::future<void>> ModelLoader::futures;
std::mutex ModelLoader::load_mutex;
std::queue<Model*> ModelLoader::model_GPU_data_queue;

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
}

void ModelLoader::load_async_file(std::unordered_map<std::string, Model>& list_ref, const std::string& name, const std::string& file_name)
{
	GLACIER_FUNC_TIMER("Loaded " + file_name + " in... ");
	Model mod(MODEL_PATH + file_name);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(list_ref.find(name) == list_ref.cend() && "Attempted to load a duplicate model!");
	Model* mod_ref = &list_ref.emplace(name, std::move(mod)).first->second;
	model_GPU_data_queue.emplace(mod_ref);
}
void ModelLoader::load(const std::string& name, const std::string& file_name)
{
	futures.emplace_back(std::async(std::launch::async, load_async_file, std::ref(models), name, file_name));
}
void ModelLoader::load_async_pre(std::unordered_map<std::string, Model>& list_ref, const std::string& name, PREMADE_MODELS premade_model, const float& scale)
{
	Model mod(premade_model, scale);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(list_ref.find(name) == list_ref.cend() && "Attempted to load a duplicate model!");
	Model* mod_ref = &list_ref.emplace(name, std::move(mod)).first->second;
	model_GPU_data_queue.emplace(mod_ref);
}
void ModelLoader::load(const std::string& name, PREMADE_MODELS premade_model, const float& scale)
{
	futures.emplace_back(std::async(std::launch::async, load_async_pre, std::ref(models), name, premade_model, scale));
}
void ModelLoader::load_async_hgtmap(std::unordered_map<std::string, Model>& list_ref, const std::string& name, const std::string& file_name, const float& xz_size, const float& max_height, const float& u, const float& v)
{
	Model mod(file_name, xz_size, max_height, u, v);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(list_ref.find(name) == list_ref.cend() && "Attempted to load a duplicated model!");
	Model* mod_ref = &list_ref.emplace(name, std::move(mod)).first->second;
	model_GPU_data_queue.emplace(mod_ref);
}
void ModelLoader::load(const std::string& name, const std::string& file_name, const float& xz_size, const float& max_height, const float& u, const float& v)
{
	futures.emplace_back(std::async(std::launch::async, load_async_hgtmap, std::ref(models), name, file_name, xz_size, max_height, u, v));
}

void ModelLoader::load(const std::string& name, const float& xz_size, const float& u, const float& v)
{
	assert(models.find(name) == models.cend() && "Attempted to load a duplicate model!");
	models.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(xz_size, u, v));
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
			auto result = v->wait_for(std::chrono::seconds(0));
			if (result == std::future_status::ready)
			{
				futures.erase(v);
				Model::load_GPU_data(*model_GPU_data_queue.front());
				model_GPU_data_queue.pop();
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