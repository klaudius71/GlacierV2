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

	static std::vector<std::future<Model&>> futures;
	static std::mutex load_mutex;

	Model& load_async_file2(const std::string& name, const std::string& file_name, const bool& glacier);
	void load(const std::string& name, const std::string& file_name, const bool& glacier);
	Model& load_async_file(const std::string& name, const std::string& file_name);
	void load(const std::string& name, const std::string& file_name);
	Model& load_async_pre(const std::string& name, PREMADE_MODELS premade_model, const float& scale);
	void load(const std::string& name, PREMADE_MODELS premade_model, const float& scale);
	Model& load_async_hgtmap(const std::string& name, const std::string& file_name, const float& xz_size, const float& max_height, const float& u, const float& v);
	void load(const std::string& name, const std::string& file_name, const float& xz_size, const float& max_height, const float& u, const float& v);
	Model& load_async_plane(const std::string& name, const float& xz_size, const float& u, const float& v);
	void load(const std::string& name, const float& xz_size, const float& u, const float& v);
	Model& load_async_sphere(const std::string& name, const uint32_t& v_slices, const uint32_t& h_slices);
	void load(const std::string& name, const uint32_t& v_slices, const uint32_t& h_slices);

	Model* const get(const PRELOADED_MODELS model);
	Model* const get(const std::string & name);

	static void WaitForThreadsAndLoadGPUData();
	static void Terminate();

	friend class ModelLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name, const bool& glacier) { Instance().load(name, file_name, glacier); }
	static void Load(const std::string& name, const std::string& file_name) { Instance().load(name, file_name); }
	static void Load(const std::string& name, PREMADE_MODELS premade_model, const float& scale = 1.0f) { Instance().load(name, premade_model, scale); }
	static void Load(const std::string& name, const std::string& file_name, const float& xz_size, const float& max_height = UINT16_MAX, const float& u = 1.0f, const float& v = 1.0f) { Instance().load(name, file_name, xz_size, max_height, u, v); }
	static void Load(const std::string& name, const float& xz_size, const float& u, const float& v) { Instance().load(name, xz_size, u, v); }
	static void Load(const std::string& name, const uint32_t& v_slices, const uint32_t& h_slices) { Instance().load(name, v_slices, h_slices); }

	static Model* Get(const PRELOADED_MODELS model) { return instance->get(model); }
	static Model* const Get(const std::string & name) { return instance->get(name); }
};

#endif _MODEL_LOADER
