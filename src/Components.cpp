#include "gpch.h"
#include "Components.h"
#include "Renderer.h"

const glm::vec3 TransformComponent::GetWorldPosition() const
{
	return world_matrix[3];
}
const glm::vec3 TransformComponent::GetForwardVector() const
{
	const float& cos_pitch = cosf(rot.x);
	return glm::vec3{ sinf(rot.y) * cos_pitch, sinf(rot.x), cosf(rot.y) * cos_pitch };
}
const glm::vec3 TransformComponent::GetGlobalForwardVector() const
{
	return glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f) * glm::mat3(world_matrix));
}

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
	//glm::vec4 ndc = (camera.proj * glm::lookAt(camera.cam_pos, camera.cam_pos + camera.cam_dir, glm::vec3(0.0f, 1.0f, 0.0f))) * glm::vec4(pos, 1.0f);
	//ImGui::Text("ndc: %.3f %.3f %.3f %.3f", ndc.x, ndc.y, ndc.z, ndc.w);
	//const float inv_w = 1.0f / ndc.w;
	//ndc *= inv_w;
	//if (ndc.z > 1.0f)
	//{
	//	ndc.x = ndc.x < 0.0f ? 1.1f : -1.1f;
	//	ndc.y = 0.0f;
	//}
	//ImGui::Text("ndc: %.3f %.3f %.3f %.3f", ndc.x, ndc.y, ndc.z, ndc.w);
	//const glm::ivec2& viewport_size = Renderer::GetMainFramebuffer().GetSize();
	//return glm::vec2{ ((ndc.x + 1.0f) / 2.0f) * viewport_size.x, viewport_size.y - ((ndc.y + 1.0f) / 2.0f) * viewport_size.y };
}