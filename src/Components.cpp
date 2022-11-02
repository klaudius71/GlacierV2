#include "gpch.h"
#include "Components.h"

void CameraComponent::ResetProjectionToSize(CameraComponent& camera, const int& width, const int& height)
{
	camera.proj = glm::perspective(camera.fov, (float)width / height, camera.near_plane, camera.far_plane);
}
