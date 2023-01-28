#ifndef _TRANSFORM_COMPONENT
#define _TRANSFORM_COMPONENT

struct TransformComponent
{
	glm::vec3& position() { flag_changed = true; return pos; }
	glm::vec3& rotation() { flag_changed = true; return rot; }
	glm::vec3& scale() { flag_changed = true; return scl; }

	const glm::mat4& GetWorldMatrix() const;
	void SetWorldMatrix(const glm::mat4& mat);
	void SetWorldMatrixKeepScale(const glm::mat4& mat);

	TransformComponent() = default;
	TransformComponent(TransformComponent&& o) = default;
	TransformComponent& operator=(TransformComponent&& o) = default;

	const glm::vec3& GetWorldPosition() const;
	const glm::vec3 GetForwardVector() const;
	const glm::vec3 GetGlobalForwardVector() const;

private:
	glm::mat4 world_matrix = glm::mat4(1.0f);
	bool flag_changed = true;
	uint8_t pad0 = 0;
	uint8_t pad1 = 0;
	uint8_t pad2 = 0;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rot = glm::vec3(0.0f);
	glm::vec3 scl = glm::vec3(1.0f);

	friend class Entity;
	friend class Renderer2D;
	friend class EditorLayer;
};

#endif