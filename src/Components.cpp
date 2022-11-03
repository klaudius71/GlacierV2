#include "gpch.h"
#include "Components.h"

const glm::vec3 TransformComponent::GetForwardVector() const
{
	const float& cos_pitch = cosf(rot.x);
	return glm::vec3{ sinf(rot.y) * cos_pitch, sinf(rot.x), cosf(rot.y) * cos_pitch };
}

void CameraComponent::ResetProjectionToSize(CameraComponent& camera, const int& width, const int& height)
{
	camera.proj = glm::perspective(camera.fov, (float)width / (float)height, camera.near_plane, camera.far_plane);
}