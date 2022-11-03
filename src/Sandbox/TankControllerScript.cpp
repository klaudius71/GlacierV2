#include "gpch.h"
#include "TankControllerScript.h"

void TankControllerScript::OnSceneEnter()
{
	assert(GetGameObject()->HasComponent<CameraComponent>());
	tank_turret = GetGameObject()->GetChild(1);
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

	glm::quat next_rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	if (Input::GetMouseButtonState(GLACIER_MOUSE::RIGHT_MOUSE_BUTTON))
	{
		const glm::vec2& mouse_delta = Input::GetMouseDeltaPosition();
		cam_rotation_euler.x += mouse_delta.y * dt;
		cam_rotation_euler.y -= mouse_delta.x * dt;
	}
	else
	{
		if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_UP))
			cam_rotation_euler.x += dt * 2.0f;
		else if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_DOWN))
			cam_rotation_euler.x -= dt * 2.0f;
		if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_LEFT))
			cam_rotation_euler.y += dt * 2.0f;		
		else if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_RIGHT))
			cam_rotation_euler.y -= dt * 2.0f;
	}
	
	const float pi = glm::pi<float>();
	if (cam_rotation_euler.y < -pi)
		cam_rotation_euler.y += pi * 2.0f;
	else if (cam_rotation_euler.y > pi)
		cam_rotation_euler.y -= pi * 2.0f;
	
	if (glm::degrees(cam_rotation_euler.x) < -35.0f)
		cam_rotation_euler.x = glm::radians(-35.0f);
	else if (glm::degrees(cam_rotation_euler.x) > 20.0f)
		cam_rotation_euler.x = glm::radians(20.0f);

	const glm::vec3 position_offset = (transform.position() + glm::vec3(0.0f, 20.0f, 0.0f)) + transform.GetForwardVector() * 22.0f;
	CameraComponent& camera = GetGameObject()->GetComponent<CameraComponent>();
	const float& cos_pitch = cosf(cam_rotation_euler.x);
	glm::vec3 camera_offset(sinf(cam_rotation_euler.y) * cos_pitch, sinf(cam_rotation_euler.x), cosf(cam_rotation_euler.y) * cos_pitch);
	camera_offset *= CAM_OFFSET;
	camera.cam_pos = position_offset - camera_offset;
	camera.cam_dir = position_offset - camera.cam_pos;

	auto& tank_turret_transform = tank_turret->GetComponent<TransformComponent>();
	glm::vec3 turret_world_rotation = transform.rotation() + tank_turret_transform.rotation();
	ImGui::Begin("Debug");
	ImGui::DragFloat3("transform.rotation()", glm::value_ptr(transform.rotation()), 0.1f);
	ImGui::DragFloat3("tank_turret_transform.rotation()", glm::value_ptr(tank_turret_transform.rotation()), 0.01f);
	ImGui::DragFloat3("turret_world_rotation", glm::value_ptr(turret_world_rotation));
	ImGui::DragFloat3("cam_rotation_euler", glm::value_ptr(cam_rotation_euler));

	static bool lock = false;
	ImGui::Checkbox("Lock Turret Rotation", &lock);

	const float camera_turret_rotation_angle = turret_world_rotation.y - cam_rotation_euler.y;
	ImGui::LabelText("", "camera_turret_rotation_angle: %.3f", camera_turret_rotation_angle);
	ImGui::End();

	if (!lock)
	{
		if (camera_turret_rotation_angle < -dt * 2.0f)
			tank_turret_transform.rotation().y += dt;
		else if (camera_turret_rotation_angle > dt * 2.0f)
			tank_turret_transform.rotation().y -= dt;

		if (tank_turret_transform.rotation().y > glm::radians(135.0f))
			tank_turret_transform.rotation().y = glm::radians(135.0f);
		else if (tank_turret_transform.rotation().y < glm::radians(-135.0f))
			tank_turret_transform.rotation().y = glm::radians(-135.0f);
	}
}

void TankControllerScript::OnScreenResize(const int& width, const int& height)
{
	CameraComponent::ResetProjectionToSize(GetGameObject()->GetComponent<CameraComponent>(), width, height);
}
