#ifndef _CAMERA_CONTROLLER_SCRIPT
#define _CAMERA_CONTROLLER_SCRIPT

#include "Script.h"

struct CameraComponent;

class CameraControllerScript : public Script
{
public:
	CameraControllerScript() = default;
	~CameraControllerScript() = default;

	virtual void OnCreate() override;
	virtual void OnTick() override;
	
private:
	const glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	CameraComponent* cam = nullptr;
	glm::vec3 camPos = glm::vec3(0.0f, 60.0f, 2.0f);
	glm::vec3 camRot = glm::vec3(0.0f, glm::pi<float>(), 0.0f);
	float cam_speed = 300.0f;
};

#endif _CAMERA_CONTROLLER_SCRIPT