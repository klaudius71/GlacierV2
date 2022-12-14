#ifndef _Timer
#define _Timer

#include <chrono>

#define GLACIER_FUNC_TIMER(expression) Timer tmr_func(expression)
#define GLACIER_LOG_FUNC_TIMER(expression) Timer tmr_func(expression, true)
#define GLACIER_TIMER_BEGIN(expression) Timer* tmr(new Timer(expression))
#define GLACIER_LOG_TIMER_BEGIN(expression) Timer* tmr(new Timer(expression, true))
#define GLACIER_TIMER_END delete tmr

#if _DEBUG
#define GLACIER_DEBUG_FUNC_TIMER(expression) Timer tmr_func(expression)
#define GLACIER_DEBUG_TIMER_BEGIN(expression) Timer* tmr(new Timer(expression))
#define GLACIER_DEBUG_LOG_FUNC_TIMER(expression) Timer tmr_func(expression, true)
#define GLACIER_DEBUG_LOG_TIMER_BEGIN(expression) Timer* tmr(new Timer(expression, true))
#define GLACIER_DEBUG_TIMER_END delete tmr
#else
#define GLACIER_DEBUG_FUNC_TIMER(expression) 
#define GLACIER_DEBUG_TIMER_BEGIN(expression)
#define GLACIER_DEBUG_LOG_FUNC_TIMER(expression)
#define GLACIER_DEBUG_LOG_TIMER_BEGIN(expression)
#define GLACIER_DEBUG_TIMER_END
#endif

class Timer
{
public:
	Timer() = delete;
	Timer(const std::string& message, bool should_log = false);
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	~Timer();

private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	std::string msg;
	bool should_log;
};


#endif _Timer