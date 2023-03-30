#include "gpch.h"
#include "CameraComponent.h"
#include "Renderer.h"

CameraComponent::CameraComponent(const glm::mat4& proj, const glm::vec3& cam_pos, const glm::vec3& cam_dir, const float fov, const float near_plane, const float far_plane)
	: proj(proj), cam_pos(cam_pos), cam_dir(cam_dir), fov(fov), near_plane(near_plane), far_plane(far_plane)
{}

const glm::mat4 CameraComponent::MakeViewMatrix() const
{
	return glm::lookAt(cam_pos, cam_pos + cam_dir, glm::vec3(0.0f, 1.0f, 0.0f));
}

void CameraComponent::ResetProjectionToSize(CameraComponent& camera, const int& width, const int& height)
{
	camera.proj = glm::perspective(camera.fov, (float)width / (float)height, camera.near_plane, camera.far_plane);
}
const glm::vec2 CameraComponent::WorldPositionToScreenPosition(const CameraComponent& camera, const glm::vec3& pos)
{
	const glm::ivec2& viewport_size = Renderer::GetMainFramebuffer().GetSize();
	const glm::mat4 modelview = camera.MakeViewMatrix() * glm::translate(glm::mat4(1.0f), pos);
	const glm::vec3 temp = glm::project(glm::vec3(0.0f), modelview, camera.proj, glm::vec4(0, 0, viewport_size));
	return glm::vec2{ temp.x, temp.y - viewport_size.y };
}