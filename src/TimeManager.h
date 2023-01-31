#ifndef _TIME_MANAGER
#define _TIME_MANAGER

#include "GlacierCore.h"

class GLACIER_API TimeManager
{
private:
	static TimeManager* instance;
	TimeManager();
	TimeManager(const TimeManager&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;
	TimeManager(TimeManager&&) = delete;
	TimeManager& operator=(TimeManager&&) = delete;
	~TimeManager() = default;

	std::chrono::time_point<std::chrono::steady_clock> prev_time;
	float delta;

	void process_time();
	void limit_frametime(const float& frame_time);

	static void Initialize();
	static void ProcessTime() { instance->process_time(); }
	static void LimitFrametime(const float& frame_time) { instance->limit_frametime(frame_time); }
	static void Terminate();

	friend class TimeManagerAtt;

public:
	static const float& GetDeltaTime() { return instance->delta; }
};

#endif _TIME_MANAGER