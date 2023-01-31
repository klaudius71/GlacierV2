#include "gpch.h"
#include "SkeletalAnimationLoader.h"
#include "Timer.h"

SkeletalAnimationLoader* SkeletalAnimationLoader::instance = nullptr;
const std::string SkeletalAnimationLoader::ANIMATION_PATH = "animations/";
std::vector<std::future<void>> SkeletalAnimationLoader::futures;
std::mutex SkeletalAnimationLoader::load_mutex;

SkeletalAnimationLoader::SkeletalAnimationLoader()
	: animations()
{
	futures.reserve(10);
}

void SkeletalAnimationLoader::load_async(const std::string& name, const std::string& file_name)
{
	GLACIER_DEBUG_FUNC_TIMER("Loaded " + file_name + " in... ");
	SkeletalAnimation sa(ANIMATION_PATH + file_name);
	std::lock_guard<std::mutex> lock(load_mutex);
	assert(animations.find(name) == animations.end());
	animations.emplace(name, std::move(sa));
}
void SkeletalAnimationLoader::load(const std::string& name, const std::string& file_name)
{
	futures.emplace_back(std::async(std::launch::async, &SkeletalAnimationLoader::load_async, this, name, file_name));
}

const SkeletalAnimation* SkeletalAnimationLoader::get(const std::string& name)
{
	auto it = animations.find(name);
	assert(it != animations.end() && "Animation not found!");
	return &it->second;
}

void SkeletalAnimationLoader::WaitForThreads()
{
	while (futures.size() != 0)
		futures.pop_back();
}
void SkeletalAnimationLoader::Terminate()
{
	delete instance;
	instance = nullptr;
}