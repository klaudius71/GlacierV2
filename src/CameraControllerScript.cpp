#include "gpch.h"
#include "CameraControllerScript.h"

CameraControllerScript::CameraControllerScript()
	: Script("CameraControllerScript")
{
}

void CameraControllerScript::OnSceneEnter()
{
	cam = &GetGameObject()->GetComponent<CameraComponent>();
}
void CameraControllerScript::OnUpdate(float dt)
{
	if (Input::GetMouseButtonState(GLACIER_MOUSE::RIGHT_MOUSE_BUTTON)) {
		camRot.x += Input::GetMouseDeltaPosition().y * dt;
		camRot.y -= Input::GetMouseDeltaPosition().x * dt;
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

	if(Input::GetKeyDown(GLACIER_KEY::KEY_LEFT_SHIFT))
		cam_speed = 600.0f;
	else
		cam_speed = 300.0f;

	const auto speed_delta = cam_speed * dt;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_W))
		cam->cam_pos += dir * speed_delta;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_S))
		cam->cam_pos -= dir * speed_delta;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_A))
		cam->cam_pos -= right * speed_delta;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_D))
		cam->cam_pos += right * speed_delta;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_Q))
		cam->cam_pos -= up * speed_delta;
	else if (Input::GetKeyDown(GLACIER_KEY::KEY_E))
		cam->cam_pos += up * speed_delta;

	const Font& font = FontLoader::Get("CascadiaMono20");
	const Framebuffer& fb = Renderer::GetMainFramebuffer();
	Renderer2D::PrintText(font, 0.0f, fb.GetSize().y - (font.GetGlyphWithMaxHeight().size.y * 0.5f) * 3.0f, Colors::Black, "Camera Controls:");
	Renderer2D::PrintText(font, 0.0f, fb.GetSize().y - (font.GetGlyphWithMaxHeight().size.y * 0.5f) * 5.0f, Colors::Black, "Movement: WASD");
	Renderer2D::PrintText(font, 0.0f, fb.GetSize().y - (font.GetGlyphWithMaxHeight().size.y * 0.5f) * 7.0f, Colors::Black, "View: RMB + Mouse");
}

void CameraControllerScript::OnScreenResize(const int& width, const int& height)
{
	CameraComponent::ResetProjectionToSize(*cam, width, height);
}