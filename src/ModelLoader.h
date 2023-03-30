#ifndef _MODEL_LOADER
#define _MODEL_LOADER

#include "GlacierCore.h"
#include "VertexTypes.h"
#include "Model.h"

enum class PRELOADED_MODELS
{
	QUAD,
	UNIT_CUBE
};

class GLACIER_API ModelLoader
{
private:
	static const std::string MODEL_PATH;

	static ModelLoader* instance;
	static ModelLoader& Instance()
	{
		if (!instance)
			instance = new ModelLoader;
		return *instance;
	}
	ModelLoader();
	ModelLoader(const ModelLoader&) = delete;
	ModelLoader& operator=(const ModelLoader&) = delete;
	ModelLoader(ModelLoader&&) = delete;
	ModelLoader& operator=(ModelLoader&&) = delete;
	~ModelLoader() = default;

	std::unordered_map<PRELOADED_MODELS, Model> preloaded_models;
	std::unordered_map<std::string, Model> models;

	std::vector<std::future<Model&>> futures;
	std::mutex load_mtx;

	Model& load_async_file(const std::string& name, const std::string& file_name);
	void load(const std::string& name, const std::string& file_name);
	Model& load_async_pre(const std::string& name, PREMADE_MODELS premade_model, float scale);
	void load(const std::string& name, PREMADE_MODELS premade_model, float scale);
	Model& load_async_hgtmap(const std::string& name, const std::string& file_name, float xz_size, float max_height, float u, float v);
	void load(const std::string& name, const std::string& file_name, float xz_size, float max_height, float u, float v);
	Model& load_async_plane(const std::string& name, float xz_size, float u, float v);
	void load(const std::string& name, float xz_size, float u, float v);
	Model& load_async_sphere(const std::string& name, uint32_t v_slices, uint32_t h_slices);
	void load(const std::string& name, uint32_t v_slices, uint32_t h_slices);

	const Model* const get(const PRELOADED_MODELS model) const;
	const Model* const get(const std::string & name) const;

	void wait_for_threads_and_load_gpu_data();

	static void WaitForThreadsAndLoadGPUData() { Instance().wait_for_threads_and_load_gpu_data(); }
	static void Terminate();

	friend class ModelLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name) { Instance().load(name, file_name); }
	static void Load(const std::string& name, PREMADE_MODELS premade_model, float scale = 1.0f) { Instance().load(name, premade_model, scale); }
	static void Load(const std::string& name, const std::string& file_name, float xz_size, float max_height = (float)UINT16_MAX, float u = 1.0f, float v = 1.0f) { Instance().load(name, file_name, xz_size, max_height, u, v); }
	static void Load(const std::string& name, float xz_size, float u, float v) { Instance().load(name, xz_size, u, v); }
	static void Load(const std::string& name, uint32_t v_slices, uint32_t h_slices) { Instance().load(name, v_slices, h_slices); }

	static const Model* Get(const PRELOADED_MODELS model) { return instance->get(model); }
	static const Model* const Get(const std::string & name) { return instance->get(name); }
};

#endif _MODEL_LOADER
