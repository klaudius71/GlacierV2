#include "gpch.h"
#include "TimeManager.h"
#include "Glacier.h"
#include "Window.h"

TimeManager* TimeManager::instance = nullptr;

void TimeManager::process_time()
{
	const auto curr_time = std::chrono::steady_clock::now();
	delta = (std::chrono::duration<float>(curr_time - prev_time)).count();
	prev_time = curr_time;
	glfwSetWindowTitle(Glacier::GetWindow().GetNativeWindow(), std::string(std::to_string(delta * 1000.0f) + "ms").c_str());
}
void TimeManager::limit_frametime(const float& frame_time)
{
	std::chrono::steady_clock::time_point curr_time = std::chrono::steady_clock::now();
	while ((std::chrono::duration<float>(curr_time - prev_time)).count() < frame_time)
		curr_time = std::chrono::steady_clock::now();
}

void TimeManager::Initialize()
{
	assert(!instance);
	instance = new TimeManager;
}
void TimeManager::Terminate()
{
	delete instance;
	instance = nullptr;
}
