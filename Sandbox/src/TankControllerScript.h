#pragma once

#include "GlacierAPI.h"

class TankControllerScript : public Script
{
public:
	TankControllerScript() = default;
	~TankControllerScript() = default;

	virtual void OnSceneEnter() override;
	virtual void OnUpdate(float dt) override;

	virtual void OnScreenResize(int width, int height) override;

private:
	const float TANK_SPEED = 100.0f;
	const float TURNING_SPEED = 1.0f;
	const float CAM_RADIUS = 85.0f;
	const float BARREL_ANGLE_OFFSET = 0.25f;

	glm::vec3 cam_rotation_euler{ 0.0f, 0.0f, 0.0f };
	GameObject tank_turret;
	GameObject tank_turret_barrel;
	GameObject crosshair;
};