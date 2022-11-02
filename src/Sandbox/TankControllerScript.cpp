#include "gpch.h"
#include "TankControllerScript.h"

void TankControllerScript::OnSceneEnter()
{
	assert(GetGameObject()->HasComponent<CameraComponent>());
	cam_rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}
void TankControllerScript::OnUpdate(float dt)
{
	TransformComponent& transform = GetGameObject()->GetComponent<TransformComponent>();

	if (Input::GetKeyDown(GLACIER_KEY::KEY_W))
		transform.position() += transform.GetForwardVector() * dt * TANK_SPEED;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_S))
		transform.position() -= transform.GetForwardVector() * dt * TANK_SPEED;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_A))
		transform.rotation().y += dt * TURNING_SPEED;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_D))
		transform.rotation().y -= dt * TURNING_SPEED;

	if (Input::GetMouseButtonState(GLACIER_MOUSE::RIGHT_MOUSE_BUTTON))
	{
		cam_rotation *= glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), Input::GetMouseDeltaPosition().y * dt, glm::vec3(1.0f, 0.0f, 0.0f) * cam_rotation);
		cam_rotation *= glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), Input::GetMouseDeltaPosition().x * -dt, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_UP))
		cam_rotation *= glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), dt * 2.0f, glm::vec3(1.0f, 0.0f, 0.0f) * cam_rotation);
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_DOWN))
		cam_rotation *= glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), -dt * 2.0f, glm::vec3(1.0f, 0.0f, 0.0f) * cam_rotation);
	if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_LEFT))
		cam_rotation *= glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), -dt * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_RIGHT))
		cam_rotation *= glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), dt * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	const glm::vec3 position_offset = (transform.position() + glm::vec3(0.0f, 20.0f, 0.0f)) + transform.GetForwardVector() * 22.0f;
	CameraComponent& camera = GetGameObject()->GetComponent<CameraComponent>();
	camera.cam_pos = position_offset + CAM_OFFSET * cam_rotation;
	camera.cam_dir = position_offset - camera.cam_pos;
}

void TankControllerScript::OnScreenResize(const int& width, const int& height)
{
	CameraComponent::ResetProjectionToSize(GetGameObject()->GetComponent<CameraComponent>(), width, height);
}
