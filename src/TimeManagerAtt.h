#ifndef _TIME_MANAGER_ATT
#define _TIME_MANAGER_ATT

#include "TimeManager.h"

class TimeManagerAtt
{
	friend class Glacier;

	static void Initialize() { TimeManager::Initialize(); }
	static void ProcessTime() { TimeManager::ProcessTime(); }
	static void LimitFrametime(const float& frame_time) { TimeManager::LimitFrametime(frame_time); }
	static void Terminate() { TimeManager::Terminate(); }
};

#endif _TIME_MANAGER_ATT