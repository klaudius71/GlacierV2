#ifndef _BONE
#define _BONE

class Bone
{
public:
	Bone() = default;
	Bone(const Bone&) = delete;
	Bone& operator=(const Bone&) = delete;
	Bone(Bone&& o);
	Bone& operator=(Bone&& o);
	~Bone() = default;

	//void ApplyTransform(glm::mat4& bone_matrix, const glm::mat4& parent_transform);
	void ApplyTransformHierarchy(const float& timestamp, std::vector<glm::mat4>& bone_matrices, const glm::mat4 parent_transform);

private:
	void ReadHierarchy(const uint32_t gltf_node, const std::vector<uint32_t>& node_to_id, const nlohmann::json& j, const std::vector<uint8_t>& buffer_data);

	std::string name = "Unnamed Bone";
	uint32_t id = 0xffffffff;

	std::vector<Bone> children;

	glm::mat4 local_transform = glm::mat4(1.0f);
	std::vector<float> position_timestamps;
	std::vector<glm::vec3> positions;
	std::vector<float> rotation_timestamps;
	std::vector<glm::quat> rotations;
	std::vector<float> scale_timestamps;
	std::vector<glm::vec3> scales;

	friend class SkeletalAnimation;
};

#endif _BONE