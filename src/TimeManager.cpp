#include "gpch.h"
#include "TimeManager.h"
#include "Glacier.h"
#include "Window.h"

TimeManager* TimeManager::instance = nullptr;

TimeManager::TimeManager()
	: prev_time(std::chrono::steady_clock::now()), delta(0.0f)
{}

void TimeManager::process_time()
{
	const auto curr_time = std::chrono::steady_clock::now();
	delta = (std::chrono::duration<float>(curr_time - prev_time)).count();
	prev_time = curr_time;
}
void TimeManager::limit_frametime(const float& frame_time)
{
	std::chrono::time_point wait_for_time = prev_time + std::chrono::microseconds((int32_t)(frame_time * 1000000.0f));
	while (std::chrono::steady_clock::now() < wait_for_time);
	//std::chrono::steady_clock::time_point curr_time = std::chrono::steady_clock::now();
	//while ((std::chrono::duration<float>(curr_time - prev_time)).count() < frame_time)
	//	curr_time = std::chrono::steady_clock::now();
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
