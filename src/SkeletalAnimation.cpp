#include "gpch.h"
#include "SkeletalAnimation.h"
#include "Bone.h"

SkeletalAnimation::SkeletalAnimation(const std::string& file_name)
	: bone_hierarchy_root(new Bone)
{
	name = file_name;
	std::ifstream file(file_name);
	assert(file.is_open());
	const nlohmann::json j = std::move(nlohmann::json::parse(file, nullptr, false, true));
	file.close();

	const std::string& uri = j["buffers"][0]["uri"];
	const std::vector<uint8_t> buffer_data = base64_decode_bytes(uri.substr(uri.find(',') + 1));
	assert(buffer_data.size() == j["buffers"][0]["byteLength"]);

	const uint32_t& timeline_accessor = j["animations"][0]["samplers"][0]["input"];
	anim_duration = j["accessors"][timeline_accessor]["max"][0];

	std::map<uint32_t, uint32_t> node_to_id_map;
	uint32_t i = 0;
	for (const uint32_t& x : j["skins"][0]["joints"])
	{
		node_to_id_map.emplace(x, i);
		i++;
	}
	i = 0;
	std::vector<uint32_t> node_to_id;
	for (const auto& x : node_to_id_map)
		node_to_id.emplace_back(x.second);
	
	bone_hierarchy_root->ReadHierarchy(j["skins"][0]["joints"][0], node_to_id, j, buffer_data);
}
SkeletalAnimation::SkeletalAnimation(SkeletalAnimation&& o)
	: name(std::move(o.name)), bone_hierarchy_root(o.bone_hierarchy_root), anim_duration(o.anim_duration)
{
	o.bone_hierarchy_root = nullptr;
	o.anim_duration = -1.0f;
}
SkeletalAnimation& SkeletalAnimation::operator=(SkeletalAnimation&& o)
{
	name = std::move(o.name);
	bone_hierarchy_root = o.bone_hierarchy_root;
	anim_duration = o.anim_duration;

	o.bone_hierarchy_root = nullptr;
	o.anim_duration = -1.0f;

	return *this;
}
SkeletalAnimation::~SkeletalAnimation()
{
	delete bone_hierarchy_root;
}

void SkeletalAnimation::Apply(glm::mat4* const mats, const std::vector<glm::mat4>& inverse_bind_matrices, const float& timestamp) const
{
	bone_hierarchy_root->ApplyTransformHierarchy(timestamp, mats, inverse_bind_matrices, glm::mat4(1.0f));
}

const std::string& SkeletalAnimation::GetName() const
{
	return name;
}
const Bone& SkeletalAnimation::GetBoneHierarchy() const
{
	return *bone_hierarchy_root;
}
const float& SkeletalAnimation::GetAnimationDuration() const
{
	return anim_duration;
}
