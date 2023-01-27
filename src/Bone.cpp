#include "gpch.h"
#include "Bone.h"

Bone::Bone(Bone&& o)
	: name(std::move(o.name)), id(o.id), children(std::move(o.children)), local_transform(1.0f),
	position_timestamps(std::move(o.position_timestamps)), positions(std::move(o.positions)),
	rotation_timestamps(std::move(o.rotation_timestamps)), rotations(std::move(o.rotations)),
	scale_timestamps(std::move(o.scale_timestamps)), scales(std::move(o.scales))
{
	o.id = 0xFFFFFFFF;
}
Bone& Bone::operator=(Bone&& o)
{
	name = std::move(o.name); 
	id = o.id;
	children = std::move(o.children);
	position_timestamps = std::move(o.position_timestamps);
	rotation_timestamps = std::move(o.rotation_timestamps);
	scale_timestamps = std::move(o.scale_timestamps);
	positions = std::move(o.positions);
	rotations = std::move(o.rotations);
	scales = std::move(o.scales);

	o.id = 0xFFFFFFFF;

	return *this;
}

void Bone::ReadHierarchy(Bone* root, const uint32_t gltf_node, const std::vector<uint32_t>& node_to_id, const nlohmann::json& j, const std::vector<uint8_t>& buffer_data)
{
	name = j["nodes"][gltf_node]["name"];
	id = node_to_id[gltf_node];

	const auto& children_array = j["nodes"][gltf_node].find("children");
	if (children_array != j["nodes"][gltf_node].end())
	{
		children.reserve(children_array.value().size());
		for (uint32_t child_node_id : children_array.value())
		{
			children.emplace_back(&root[node_to_id[child_node_id]]);
			children.back()->ReadHierarchy(root, child_node_id, node_to_id, j, buffer_data);
		}
	}

	struct Channel
	{
		uint32_t sampler;
		enum class Path
		{
			KEY_POSITION,
			KEY_ROTATION,
			KEY_SCALE
		} path;
		
		Channel(const uint32_t& sampler, const Path& path)
			: sampler(sampler), path(path)
		{}
	};
	std::vector<Channel> channels;
	channels.reserve(3);
	for (const auto& x : j["animations"][0]["channels"])
	{
		const auto& target = x["target"];
		if (target["node"] == gltf_node)
		{
			const Channel::Path path = target["path"] == std::string("translation") ? Channel::Path::KEY_POSITION : target["path"] == std::string("rotation") ? Channel::Path::KEY_ROTATION : Channel::Path::KEY_SCALE;
			channels.emplace_back(x["sampler"], path);
			if (channels.size() == 3)
				break;
		}
	}

	const auto& samplers = j["animations"][0]["samplers"];
	for (const auto& x : channels)
	{
		const auto& sampler = samplers[x.sampler];
		const uint32_t& input_accessor = sampler["input"];
		const uint32_t& output_accessor = sampler["output"];

		const auto& accessors = j["accessors"];
		const auto& bufferViews = j["bufferViews"];

		const uint32_t& bufferView_timestamps = accessors[input_accessor]["bufferView"];
		const uint32_t& count_timestamps = accessors[input_accessor]["count"];
		const uint32_t& byteOffset_timestamp = bufferViews[bufferView_timestamps]["byteOffset"];
		const float* const timestamps = (float*)&buffer_data[byteOffset_timestamp];

		const uint32_t& bufferView_keyframes = accessors[output_accessor]["bufferView"];
		const uint32_t& count_keyframes = accessors[output_accessor]["count"];
		const uint32_t& byteOffset_keyframes = bufferViews[bufferView_keyframes]["byteOffset"];
		const float* const keyframes = (float*)&buffer_data[byteOffset_keyframes];

		switch (x.path)
		{
			case Channel::Path::KEY_POSITION:
			{
				position_timestamps = std::vector<float>(timestamps, timestamps + count_timestamps);
				positions = std::vector<glm::vec3>((glm::vec3*)keyframes, ((glm::vec3*)keyframes) + count_keyframes);
				break;
			}
			case Channel::Path::KEY_ROTATION:
			{
				rotation_timestamps = std::vector<float>(timestamps, timestamps + count_timestamps);
				rotations.reserve(rotation_timestamps.size());
				rotations = std::vector<glm::quat>((glm::quat*)keyframes, ((glm::quat*)keyframes) + count_keyframes);
				break;
			}
			case Channel::Path::KEY_SCALE:
			{
				scale_timestamps = std::vector<float>(timestamps, timestamps + count_timestamps);
				scales = std::vector<glm::vec3>((glm::vec3*)keyframes, ((glm::vec3*)keyframes) + count_keyframes);
				break;
			}
		}
	}
}

void Bone::ApplyTransformHierarchy(const float& timestamp, glm::mat4* const bone_matrices, const std::vector<glm::mat4>& inverse_bind_matrices, const glm::mat4 parent_transform)
{
	assert(timestamp >= 0.0f);

	size_t i;
	for (i = 0; i < position_timestamps.size() - 2; i++)
	{
		if (timestamp < position_timestamps[i + 1])
			break;
	}
	float delta = (timestamp - position_timestamps[i]) / (position_timestamps[i + 1] - position_timestamps[i]);

	local_transform = glm::translate(glm::mat4(1.0f), Tools::VectMath::Lerp2(positions[i], positions[i + 1], delta));
	
	for (i = 0; i < rotation_timestamps.size() - 2; i++)
	{
		if (timestamp < rotation_timestamps[i + 1])
			break;
	}
	delta = (timestamp - rotation_timestamps[i]) / (rotation_timestamps[i + 1] - rotation_timestamps[i]);
	
	local_transform *= glm::mat4_cast(glm::slerp(rotations[i], rotations[i + 1], delta));

	for (i = 0; i < scale_timestamps.size() - 2; i++)
	{
		if (timestamp < scale_timestamps[i + 1])
			break;
	}
	delta = (timestamp - scale_timestamps[i]) / (scale_timestamps[i + 1] - scale_timestamps[i]);

	local_transform *= glm::scale(glm::mat4(1.0f), Tools::VectMath::Lerp2(scales[i], scales[i + 1], delta));

	const glm::mat4& transform = parent_transform * local_transform;

	bone_matrices[id] = transform * inverse_bind_matrices[id];

	for (auto& x : children)
		x->ApplyTransformHierarchy(timestamp, bone_matrices, inverse_bind_matrices, transform);
}