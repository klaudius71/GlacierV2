#ifndef _VERTEX_TYPES
#define _VERTEX_TYPES

#define MAX_BONES 100

#include "GlacierCore.h"
#include "glm/glm.hpp"

namespace VertexTypes
{
	struct GLACIER_API Vertex
	{
		glm::vec3 pos{ 0.0f, 0.0f, 0.0f };
		glm::vec2 uv{ 0.0f, 0.0f };
		uint32_t tex_id = 0;
		glm::vec3 normal{ 0.0f, 1.0f, 0.0f };
		glm::vec3 tangent{ 0.0f, 0.0f, 1.0f };
		glm::vec3 bitangent{ 1.0f, 0.0f, 0.0f };
		glm::uvec4 joint_ids{ 0, 0, 0, 0 };
		glm::vec4 joint_weights{ 1.0f, 1.0f, 1.0f, 1.0f };

		Vertex(const float& pos_x = 0.0f, const float& pos_y = 0.0f, const float& pos_z = 0.0f,
			const float& u = 0.0f, const float& v = 0.0f, const uint32_t& tex_id = 0,
			const float& normal_x = 0.0f, const float& normal_y = 1.0f, const float& normal_z = 0.0f,
			const float& tangent_x = 1.0f, const float& tangent_y = 0.0f, const float& tangent_z = 0.0f,
			const float& bitangent_x = 0.0f, const float& bitangent_y = 0.0f, const float& bitangent_z = 1.0f,
			const uint32_t& joints_ids0 = 0, const uint32_t& joints_ids1 = 0, const uint32_t& joints_ids2 = 0, const uint32_t& joints_ids3 = 0,
			const float& joint_weights0 = 1.0f, const float& joint_weights1 = 1.0f, const float& joint_weights2 = 1.0f, const float& joint_weights3 = 1.0f)
			: pos(pos_x, pos_y, pos_z), uv(u, v), tex_id(tex_id),
			normal(normal_x, normal_y, normal_z), tangent(tangent_x, tangent_y, tangent_z), bitangent(bitangent_x, bitangent_y, bitangent_z), 
			joint_ids(joints_ids0, joints_ids1, joints_ids2, joints_ids3), joint_weights(joint_weights0, joint_weights1, joint_weights2, joint_weights3)
		{}
		Vertex(const glm::vec3& pos, const glm::vec2& uv = glm::vec2(0.0f), const uint32_t& tex_id = 0, 
			const glm::vec3& normal = glm::vec3(0.0f, 1.0f, 0.0f), const glm::vec3& tangent = glm::vec3(1.0f, 0.0f, 0.0f), const glm::vec3& bitangent = glm::vec3(0.0f, 0.0f, 1.0f),
			const glm::uvec4& joint_ids = glm::uvec4(0), const glm::vec4& joint_weights = glm::vec4(1.0f))
			: pos(pos), uv(uv), tex_id(tex_id), normal(normal), tangent(tangent), bitangent(bitangent), joint_ids(joint_ids), joint_weights(joint_weights)
		{}

		void set(const glm::vec3& _pos, const glm::vec2& _uv, const glm::vec3& _normal)
		{
			pos = _pos;
			uv = _uv;
			normal = _normal;
		}
	};
	struct GLACIER_API VertexTriangle
	{
		uint32_t i0 = 0;
		uint32_t i1 = 0;
		uint32_t i2 = 0;

		VertexTriangle(const uint32_t& i0 = 0, const uint32_t& i1 = 0, const uint32_t& i2 = 0)
			: i0(i0), i1(i1), i2(i2)
		{}


	};

	struct GLACIER_API ALIGN16 PhongADS
	{
		glm::vec3 ambient = glm::vec3(1.0f);
		float pad0 = 0.0f;
		glm::vec3 diffuse = glm::vec3(1.0f);
		float pad1 = 0.0f;
		glm::vec3 specular = glm::vec3(1.0f);
		float shininess = 32.0f;

		PhongADS() = default;
		PhongADS(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float& shininess)
			: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
		{}
		PhongADS(const PhongADS&) = default;
	};
	struct GLACIER_API ALIGN16 DirectionalLight
	{
		PhongADS light_properties;
		glm::vec3 direction = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)); // world direction
		float pad0 = 0.0f;

		DirectionalLight() = default;
		DirectionalLight(const PhongADS& ads, const glm::vec3 dir)
			: light_properties(ads), direction(dir)
		{}
	};

#if GLACIER_DIRECTX
	struct ALIGN16 CamData
	{
		glm::mat4 Projection;
		glm::mat4 View;
	};
	struct ALIGN16 InstanceData
	{
		glm::mat4 World;
	};
	struct ALIGN16 SpriteData
	{
		glm::vec2 TexelPos;
		glm::vec2 Size;
		glm::vec4 Color;
		glm::vec2 TexSize;
	};
	struct ALIGN16 JointData
	{
		glm::mat4 JointMatrices[MAX_BONES];
	};
	struct ALIGN16 LightspaceData
	{
		glm::mat4 Lightspace;
	};
#endif
}

#endif