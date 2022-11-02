#ifndef _CAMERA_CONTROLLER_SCRIPT
#define _CAMERA_CONTROLLER_SCRIPT

#include "GlacierAPI.h"

class CameraControllerScript : public Script
{
public:
	CameraControllerScript();
	~CameraControllerScript() = default;

	virtual void OnSceneEnter() override;
	virtual void OnUpdate(float dt) override;
	
	virtual void OnScreenResize(const int& width, const int& height);
	
private:
	const glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	CameraComponent* cam = nullptr;
	glm::vec3 camRot = glm::vec3(0.0f, glm::pi<float>(), 0.0f);
	float cam_speed = 300.0f;
};

#endif _CAMERA_CONTROLLER_SCRIPT