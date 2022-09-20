#ifndef _Timer
#define _Timer

#include <chrono>

class Timer
{
public:
	Timer() = delete;
	Timer(const std::string& message);
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	~Timer();

private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	std::string msg;
};

#define GLACIER_FUNC_TIMER(expression) Timer tmr_func(expression)
#define GLACIER_TIMER_BEGIN(expression) Timer* tmr(new Timer(expression))
#define GLACIER_TIMER_END delete tmr

#endif _Timer