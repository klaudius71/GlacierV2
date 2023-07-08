#include "gpch.h"
#include "TransformComponent.h"

const glm::mat4& TransformComponent::GetWorldMatrix() const
{
	return world_matrix;
}

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