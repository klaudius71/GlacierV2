#ifndef _SKELETAL_ANIMATION_LOADER
#define _SKELETAL_ANIMATION_LOADER

class SkeletalAnimation;

class SkeletalAnimationLoader
{
private:
	static const std::string ANIMATION_PATH;

	static SkeletalAnimationLoader* instance;
	static SkeletalAnimationLoader& Instance()
	{
		if (!instance)
			instance = new SkeletalAnimationLoader;
		return *instance;
	}
	SkeletalAnimationLoader();
	SkeletalAnimationLoader(const SkeletalAnimationLoader&) = delete;
	SkeletalAnimationLoader& operator=(const SkeletalAnimationLoader&) = delete;
	SkeletalAnimationLoader(SkeletalAnimationLoader&&) = delete;
	SkeletalAnimationLoader& operator=(SkeletalAnimationLoader&&) = delete;
	~SkeletalAnimationLoader() = default;

	std::unordered_map<std::string, SkeletalAnimation> animations;

	static std::vector<std::future<void>> futures;
	static std::mutex load_mutex;

	void load_async(const std::string& name, const std::string& file_name);
	void load(const std::string& name, const std::string& file_name);

	const SkeletalAnimation* get(const std::string& name);

	static void WaitForThreads();
	static void Terminate();

	friend class SkeletalAnimationLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name) { Instance().load(name, file_name); }

	static const SkeletalAnimation* Get(const std::string& name) { assert(instance && "SkeletalAnimationLoader not initialized!"); return instance->get(name); }
};

#endif _SKELETAL_ANIMATION_LOADER