#include "gpch.h"
#include "Timer.h"
#include "Logger.h"

Timer::Timer(const std::string& message, bool should_log)
	: duration(0.0f), msg(message), should_log(should_log), start(std::chrono::high_resolution_clock::now())
{
}
Timer::~Timer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	float ms = duration.count() * 1000.0f;

	if (should_log)
		GLACIER_LOG(msg.c_str(), ms);
	else
		printf("%s%fms\n", msg.c_str(), ms);

}