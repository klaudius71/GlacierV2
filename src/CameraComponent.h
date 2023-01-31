#ifndef _CAMERA_COMPONENT
#define _CAMERA_COMPONENT

#include "GlacierCore.h"

struct GLACIER_API CameraComponent
{
	glm::vec3 cam_pos = glm::vec3(0.0f);
	glm::vec3 cam_dir = glm::vec3(0.0f, 0.0f, 1.0f);
	float fov = glm::radians(90.0f);
	float near_plane = 0.1f;
	float far_plane = 10000.0f;
	glm::mat4 proj = glm::perspective(fov, 16.0f / 9.0f, near_plane, far_plane);

	CameraComponent() = default;
	CameraComponent(const glm::mat4& proj, const glm::vec3& cam_pos = glm::vec3(0.0f), const glm::vec3& cam_dir = glm::vec3(0.0f, 0.0f, -1.0f), const float& fov = glm::radians(90.0f), const float& near_plane = 0.1f, const float& far_plane = 10000.0f);
	CameraComponent(CameraComponent&& o) = default;
	CameraComponent& operator=(CameraComponent&& o) = default;
	~CameraComponent() = default;

	const glm::mat4 MakeViewMatrix() const;

	static void ResetProjectionToSize(CameraComponent& camera, const int& width, const int& height);
	static const glm::vec2 WorldPositionToScreenPosition(const CameraComponent& camera, const glm::vec3& pos);
};

#endif