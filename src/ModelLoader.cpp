#include "gpch.h"
#include "ModelLoader.h"
#include "Model.h"
#include "ModelAtt.h"

ModelLoader* ModelLoader::instance = nullptr;
const std::string ModelLoader::MODEL_PATH = "assets/models/";

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
	auto mod = &preloaded_models.emplace(std::piecewise_construct, std::forward_as_tuple(PRELOADED_MODELS::QUAD), std::forward_as_tuple(verts, tris)).first->second;
	ModelAtt::LoadGPUData(*mod);
	verts.clear();
	tris.clear();

	mod = &preloaded_models.emplace(std::piecewise_construct, std::forward_as_tuple(PRELOADED_MODELS::UNIT_CUBE), std::forward_as_tuple(PREMADE_MODELS::UNIT_CUBE_REPEAT_TEXTURE, 1.0f)).first->second;
	ModelAtt::LoadGPUData(*mod);
}

#if GLACIER_OPENGL
Model& ModelLoader::load_async_file(const std::string& name, const std::string& file_name)
{
	GLACIER_DEBUG_FUNC_TIMER("Loaded " + file_name + " in... ");
	ModelOpenGL mod(MODEL_PATH + file_name);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicate model!");
	return models.emplace(name, std::move(mod)).first->second;
}
Model& ModelLoader::load_async_pre(const std::string& name, PREMADE_MODELS premade_model, float scale)
{
	ModelOpenGL mod(premade_model, scale);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicate model!");
	return models.emplace(name, std::move(mod)).first->second;
}
Model& ModelLoader::load_async_hgtmap(const std::string& name, const std::string& file_name, float xz_size, float max_height, float u, float v)
{
	ModelOpenGL mod(file_name, xz_size, max_height, u, v);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicated model!");
	return models.emplace(name, std::move(mod)).first->second;
}
Model& ModelLoader::load_async_plane(const std::string& name, float xz_size, float u, float v)
{
	ModelOpenGL mod(xz_size, u, v);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicated model!");
	return models.emplace(name, std::move(mod)).first->second;
}
Model& ModelLoader::load_async_sphere(const std::string& name, uint32_t v_slices, uint32_t h_slices)
{
	ModelOpenGL mod(v_slices, h_slices);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicated model!");
	return models.emplace(name, std::move(mod)).first->second;
}
#elif GLACIER_DIRECTX
Model& ModelLoader::load_async_file(const std::string& name, const std::string& file_name)
{
	GLACIER_DEBUG_FUNC_TIMER("Loaded " + file_name + " in... ");
	ModelDirectX mod(MODEL_PATH + file_name);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicate model!");
	return models.emplace(name, std::move(mod)).first->second;
}
Model& ModelLoader::load_async_pre(const std::string& name, PREMADE_MODELS premade_model, float scale)
{
	ModelDirectX mod(premade_model, scale);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicate model!");
	return models.emplace(name, std::move(mod)).first->second;
}
Model& ModelLoader::load_async_hgtmap(const std::string& name, const std::string& file_name, float xz_size, float max_height, float u, float v)
{
	ModelDirectX mod(file_name, xz_size, max_height, u, v);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicated model!");
	return models.emplace(name, std::move(mod)).first->second;
}
Model& ModelLoader::load_async_plane(const std::string& name, float xz_size, float u, float v)
{
	ModelDirectX mod(xz_size, u, v);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicated model!");
	return models.emplace(name, std::move(mod)).first->second;
}
Model& ModelLoader::load_async_sphere(const std::string& name, uint32_t v_slices, uint32_t h_slices)
{
	ModelDirectX mod(v_slices, h_slices);
	std::lock_guard<std::mutex> lock(load_mtx);
	assert(models.find(name) == models.cend() && "Attempted to load a duplicated model!");
	return models.emplace(name, std::move(mod)).first->second;
}
#endif

void ModelLoader::load(const std::string& name, const std::string& file_name)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_file, this, name, file_name));
}
void ModelLoader::load(const std::string& name, PREMADE_MODELS premade_model, float scale)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_pre, this, name, premade_model, scale));
}
void ModelLoader::load(const std::string& name, const std::string& file_name, float xz_size, float max_height, float u, float v)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_hgtmap, this, name, file_name, xz_size, max_height, u, v));
}
void ModelLoader::load(const std::string& name, float xz_size, float u, float v)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_plane, this, name, xz_size, u, v));
}
void ModelLoader::load(const std::string& name, uint32_t v_slices, uint32_t h_slices)
{
	futures.push_back(std::async(std::launch::async, &ModelLoader::load_async_sphere, this, name, v_slices, h_slices));
}

const Model* const ModelLoader::get(const PRELOADED_MODELS model) const
{
	return &preloaded_models.at(model);
}
const Model* const ModelLoader::get(const std::string& name) const
{
	auto it = models.find(name);
	assert(it != models.cend() && "Model not found!");
	return &it->second;
}

void ModelLoader::wait_for_threads_and_load_gpu_data()
{
	while (!futures.empty())
	{
		for (auto v = futures.begin(); v != futures.end(); ++v)
		{
			if (v->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
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