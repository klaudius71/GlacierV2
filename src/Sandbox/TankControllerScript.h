#pragma once

#include "GlacierAPI.h"

class TankControllerScript : public Script
{
public:
	TankControllerScript() = default;
	~TankControllerScript() = default;

	virtual void OnSceneEnter() override;
	virtual void OnUpdate(float dt) override;

	virtual void OnScreenResize(const int& width, const int& height) override;

private:
	const float TANK_SPEED = 100.0f;
	const float TURNING_SPEED = 1.0f;

	const float CAM_OFFSET = 85.0f;

	//glm::quat cam_rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 cam_rotation_euler{ 0.0f, 0.0f, 0.0f };
	GameObject tank_turret;
};