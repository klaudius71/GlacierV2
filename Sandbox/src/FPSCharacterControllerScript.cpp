#include "FPSCharacterControllerScript.h"

FPSCharacterControllerScript::FPSCharacterControllerScript()
	: Script("FPSCharacterController")
{
}

void FPSCharacterControllerScript::OnUpdate(float dt)
{
	CharacterControllerComponent& character_controller = GetGameObject()->GetComponent<CharacterControllerComponent>();
	CameraComponent* cam = &GetGameObject()->GetComponent<CameraComponent>();
	TransformComponent& transform = GetGameObject()->GetComponent<TransformComponent>();
	glm::vec3 rot = transform.rotation();
	
	if (Input::GetMouseButtonState(GLACIER_MOUSE::RIGHT_MOUSE_BUTTON)) 
	{
		//Glacier::GetWindow().HideCursor();
		camRot.x += Input::GetMouseDeltaPosition().y * dt;
		camRot.y -= Input::GetMouseDeltaPosition().x * dt;
	}
	else
	{
		//Glacier::GetWindow().ShowCursor();
	}
	if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_UP))
		camRot.x += 1.5f * dt;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_DOWN))
		camRot.x -= 1.5f * dt;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_LEFT))
		camRot.y += 1.5f * dt;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_ARROW_RIGHT))
		camRot.y -= 1.5f * dt;

	camRot.x = std::clamp(camRot.x, glm::radians(-89.0f), glm::radians(89.0f));

	const float& cos_pitch = cosf(camRot.x);
	const glm::vec3 dir(sinf(camRot.y) * cos_pitch, sinf(camRot.x), cosf(camRot.y) * cos_pitch);
	const glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::vec3 up = glm::normalize(-glm::cross(dir, right));
	cam->cam_dir = dir;

	const glm::vec3 dir_pitchless(glm::normalize(glm::vec3(dir.x, 0.0f, dir.z)));
	glm::vec3 walk_direction = glm::vec3(0.0f);
	if (Input::GetKeyDown(GLACIER_KEY::KEY_W))
		walk_direction += dir_pitchless;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_S))
		walk_direction -= dir_pitchless;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_A))
		walk_direction -= right;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_D))
		walk_direction += right;
	
	if(walk_direction != glm::vec3(0.0f))
		glm::normalize(walk_direction);

	//character_controller.ApplyCentralImpulse(walk_direction * 300.0f);
	character_controller.AddMovement(walk_direction);

	if (Input::GetKeyDown(GLACIER_KEY::KEY_SPACE))
		character_controller.Jump();

	const glm::vec3& world_pos = transform.GetWorldPosition();
	cam->cam_pos = glm::vec3(world_pos.x, world_pos.y + 25.0f, world_pos.z);

	const Font& font = FontLoader::Get("CascadiaMono20");
	const Framebuffer& fb = Renderer::GetMainFramebuffer();
	Renderer2D::PrintText(font, 0.0f, fb.GetSize().y - (font.GetGlyphWithMaxHeight().size.y * 0.5f) * 5.0f, Colors::Black, "Camera Controls:");
	Renderer2D::PrintText(font, 0.0f, fb.GetSize().y - (font.GetGlyphWithMaxHeight().size.y * 0.5f) * 7.0f, Colors::Black, "Movement: WASD");
	Renderer2D::PrintText(font, 0.0f, fb.GetSize().y - (font.GetGlyphWithMaxHeight().size.y * 0.5f) * 9.0f, Colors::Black, "View: RMB + Mouse");
}
