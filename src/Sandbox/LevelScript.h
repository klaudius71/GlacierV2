#pragma once

#include "GlacierAPI.h"

class LevelScript : public Script
{
public:
	LevelScript();
	~LevelScript() = default;

	virtual void OnCreate() override;
	virtual void OnTick() override;
	virtual void OnDestroy() override;

private:
	GameObjectRef cube2;
	int frame_time_index;
	std::vector<float> frame_time_data;
};