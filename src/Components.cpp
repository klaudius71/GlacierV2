#include "gpch.h"
#include "Components.h"
#include "Renderer.h"

void TransformComponent::SetWorldMatrix(const glm::mat4& mat)
{
	pos = mat[3];
	glm::extractEulerAngleXYX(mat, rot.x, rot.y, rot.z);
	scl = glm::vec3(glm::length(scl[0]), glm::length(scl[1]), glm::length(scl[2]));
	world_matrix = mat;
}
void TransformComponent::SetWorldMatrixKeepScale(const glm::mat4& mat)
{
	pos = mat[3];
	glm::extractEulerAngleXYX(mat, rot.x, rot.y, rot.z);
	world_matrix = mat;
	world_matrix[0] *= scl.x;
	world_matrix[1] *= scl.y;
	world_matrix[2] *= scl.z;
}

const glm::vec3& TransformComponent::GetWorldPosition() const
{
	return *(const glm::vec3*)&world_matrix[3];
}
const glm::vec3 TransformComponent::GetForwardVector() const
{
	const float& cos_pitch = cosf(rot.x);
	return glm::vec3{ sinf(rot.y) * cos_pitch, sinf(rot.x), cosf(rot.y) * cos_pitch };
}
const glm::vec3 TransformComponent::GetGlobalForwardVector() const
{
	return glm::normalize(world_matrix[2]);
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
}