#include "TankControllerScript.h"

void TankControllerScript::OnSceneEnter()
{
	assert(GetGameObject()->HasComponent<CameraComponent>());
	tank_turret = GetGameObject()->GetChild(1);
	tank_turret_barrel = tank_turret->GetChild(0);
	crosshair = GetGameObject()->GetChild(2);
}
void TankControllerScript::OnUpdate(float dt)
{
	TransformComponent& transform = GetGameObject()->GetComponent<TransformComponent>();

	// Movement
	if (Input::GetKeyDown(GLACIER_KEY::KEY_W))
		transform.position() += transform.GetForwardVector() * dt * TANK_SPEED;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_S))
		transform.position() -= transform.GetForwardVector() * dt * TANK_SPEED;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_A))
		transform.rotation().y += dt * TURNING_SPEED;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_D))
		transform.rotation().y -= dt * TURNING_SPEED;

	// Camera Movement
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
	
	// Clamps the camera's yaw values between -180 and 180 degrees
	if (cam_rotation_euler.y < -glm::pi<float>())
		cam_rotation_euler.y += glm::pi<float>() * 2.0f;
	else if (cam_rotation_euler.y > glm::pi<float>())
		cam_rotation_euler.y -= glm::pi<float>() * 2.0f;
	
	// Camera Clamping
	if (glm::degrees(cam_rotation_euler.x) < -35.0f)
		cam_rotation_euler.x = glm::radians(-35.0f);
	else if (glm::degrees(cam_rotation_euler.x) > 20.0f)
		cam_rotation_euler.x = glm::radians(20.0f);

	// Camera view calculation
	const glm::vec3 position_offset = (transform.position() + glm::vec3(0.0f, 20.0f, 0.0f)) + transform.GetForwardVector() * 22.0f;
	const float& cos_pitch = cosf(cam_rotation_euler.x);
	const glm::vec3 camera_offset = CAM_RADIUS * glm::vec3(sinf(cam_rotation_euler.y) * cos_pitch, sinf(cam_rotation_euler.x), cosf(cam_rotation_euler.y) * cos_pitch);
	CameraComponent& camera = GetGameObject()->GetComponent<CameraComponent>();
	camera.cam_pos = position_offset - camera_offset;
	camera.cam_dir = position_offset - camera.cam_pos;

	// Independent turret rotation
	auto& tank_turret_transform_rotation = tank_turret->GetComponent<TransformComponent>().rotation();
	const glm::vec3 turret_world_rotation = transform.rotation() + tank_turret_transform_rotation;
	const float camera_turret_rotation_angle = -glm::eulerAngles(glm::quat(cam_rotation_euler) * glm::inverse(glm::quat(turret_world_rotation))).y;

	if (camera_turret_rotation_angle < -dt * 1.1f)
		tank_turret_transform_rotation.y += dt;
	else if (camera_turret_rotation_angle > dt * 1.1f)
		tank_turret_transform_rotation.y -= dt;
	
	if (tank_turret_transform_rotation.y > glm::radians(135.0f))
		tank_turret_transform_rotation.y = glm::radians(135.0f);
	else if (tank_turret_transform_rotation.y < glm::radians(-135.0f))
		tank_turret_transform_rotation.y = glm::radians(-135.0f);
	
	// Independent turret barrel rotation
	auto& tank_turret_barrel_transform = tank_turret_barrel->GetComponent<TransformComponent>();
	auto& tank_turret_barrel_transform_rotation = tank_turret_barrel_transform.rotation();
	const glm::vec3 turret_barrel_world_rotation = turret_world_rotation + tank_turret_barrel_transform_rotation;
	const float camera_turret_barrel_rotation_angle = cam_rotation_euler.x - (-turret_barrel_world_rotation.x);
	
	if (camera_turret_barrel_rotation_angle < (-dt * 1.1f - BARREL_ANGLE_OFFSET))
		tank_turret_barrel_transform_rotation.x += dt;
	else if (camera_turret_barrel_rotation_angle > (dt * 1.1f - BARREL_ANGLE_OFFSET))
		tank_turret_barrel_transform_rotation.x -= dt;

	if (tank_turret_barrel_transform_rotation.x > glm::radians(10.0f))
		tank_turret_barrel_transform_rotation.x = glm::radians(10.0f);
	else if (tank_turret_barrel_transform_rotation.x < glm::radians(-18.0f))
		tank_turret_barrel_transform_rotation.x = glm::radians(-18.0f);

	// Crosshair
	const glm::vec3 target = tank_turret_barrel_transform.GetWorldPosition() + tank_turret_barrel_transform.GetGlobalForwardVector() * 1000.0f;
	const glm::vec2 screen_pos = CameraComponent::WorldPositionToScreenPosition(camera, target);
	TransformComponent& crosshair_transform = crosshair->GetComponent<TransformComponent>();
	crosshair_transform.position() = Tools::VectMath::Lerp2(crosshair_transform.position(), glm::vec3{ screen_pos, 0.0f }, TimeManager::GetDeltaTime() * 20.0f);

	// Testing
	GameObject sphere = GetCurrentScene().FindGameObject("Sphere");
	Renderer2D::PrintText(FontLoader::Get("CascadiaMono20"), 0.0f, 20.0f, Colors::Black, "%d", sphere->GetComponent<const NameComponent>().id);
}

void TankControllerScript::OnScreenResize(int width, int height)
{
	CameraComponent::ResetProjectionToSize(GetGameObject()->GetComponent<CameraComponent>(), width, height);
}