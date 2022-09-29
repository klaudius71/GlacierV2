#include "gpch.h"
#include "CameraControllerScript.h"
#include "GameObject.h"
#include "Input.h"
#include "TimeManager.h"
#include "Glacier.h"
#include "Window.h"

void CameraControllerScript::OnCreate()
{
	cam = &GetGameObject()->GetComponent<CameraComponent>();
}
void CameraControllerScript::OnTick()
{
	if (Input::GetMouseButtonState(GLACIER_MOUSE::RIGHT_MOUSE_BUTTON)) {
		camRot.x += Input::GetMouseDeltaPosition().y * TimeManager::GetDeltaTime();
		camRot.y += Input::GetMouseDeltaPosition().x * TimeManager::GetDeltaTime();
	}
	if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_UP))
		camRot.x += 1.5f * TimeManager::GetDeltaTime();
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_DOWN))
		camRot.x -= 1.5f * TimeManager::GetDeltaTime();
	if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_LEFT))
		camRot.y += 1.5f * TimeManager::GetDeltaTime();
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_RIGHT))
		camRot.y -= 1.5f * TimeManager::GetDeltaTime();

	camRot.x = std::clamp(camRot.x, glm::radians(-89.0f), glm::radians(89.0f));

	const auto& cos_pitch = cosf(camRot.x);
	const glm::vec3 dir(sinf(camRot.y) * cos_pitch, sinf(camRot.x), cosf(camRot.y) * cos_pitch);
	const glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::vec3 up = glm::normalize(-glm::cross(dir, right));
	
	if(Input::GetKeyDown(GLACIER_KEY::KEY_LEFT_SHIFT))
		cam_speed = 600.0f;
	else
		cam_speed = 300.0f;

	const auto speed_delta = cam_speed * TimeManager::GetDeltaTime();
	if (Input::GetKeyDown(GLACIER_KEY::KEY_W))
		camPos += dir * speed_delta;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_S))
		camPos -= dir * speed_delta;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_A))
		camPos -= right * speed_delta;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_D))
		camPos += right * speed_delta;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_Q))
		camPos -= up * speed_delta;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_E))
		camPos += up * speed_delta;

	cam->view = glm::lookAt(camPos, camPos + dir, camUp);
}
