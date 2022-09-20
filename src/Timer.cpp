#include "gpch.h"
#include "Timer.h"

Timer::Timer(const std::string& message)
	: duration(0.0f), msg(message)
{
	start = std::chrono::high_resolution_clock::now();
}
Timer::~Timer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;

	float ms = duration.count() * 1000.0f;
	printf("%s%fms\n", msg.c_str(), ms);
}
