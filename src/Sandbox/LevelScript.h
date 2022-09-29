#pragma once

#include "GlacierAPI.h"

class LevelScript : public Script
{
public:
	LevelScript() = default;
	~LevelScript() = default;

	virtual void OnCreate() override;
	virtual void OnTick() override;
	virtual void OnDestroy() override;

private:
	GameObjectRef cube2;
};