#ifndef _COMPONENTS
#define _COMPONENTS

#include "Renderer2D.h"
#include "Shader.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "Model.h"
#include "Script.h"

struct NameComponent
{
	std::string name;
	size_t id;

	NameComponent(const std::string& name)
		: name(name), id(std::hash<std::string>()(name))
	{
	}
	NameComponent(std::string&& name)
		: name(std::move(name))
	{ 
		id = std::hash<std::string>()(this->name); 
	}
	NameComponent(NameComponent&& o)
		: id(o.id), name(std::move(o.name))
	{}
	NameComponent& operator=(NameComponent&& o)
	{ 
		name = std::move(o.name); 
		id = o.id;
		return *this; 
	}
};

struct TransformComponent
{
	glm::vec3& position() { flag_changed = true; return pos; }
	glm::vec3& rotation() { flag_changed = true; return rot; }
	glm::vec3& scale() { flag_changed = true; return scl; }

	const glm::mat4& GetWorldMatrix() const { return world_matrix; }

	TransformComponent() = default;
	TransformComponent(TransformComponent&& o) = default;
	TransformComponent& operator=(TransformComponent&& o) = default;

private:
	glm::mat4 world_matrix = glm::mat4(1.0f);
	bool flag_changed = true;
	uint8_t pad0 = 0;
	uint8_t pad1 = 0;
	uint8_t pad2 = 0;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rot = glm::vec3(0.0f);
	glm::vec3 scl = glm::vec3(1.0f);

	friend class GameObject;
};

struct CameraComponent
{
	glm::vec3 cam_pos = glm::vec3(0.0f);
	glm::vec3 cam_dir = glm::vec3(0.0f, 0.0f, 1.0f);
	float fov = glm::radians(90.0f);
	float near_plane = 0.1f;
	float far_plane = 10000.0f;
	glm::mat4 proj = glm::perspective(fov, 16.0f / 9.0f, near_plane, far_plane);

	CameraComponent() = default;
	CameraComponent(const glm::mat4& proj, const glm::vec3& cam_pos = glm::vec3(0.0f), const glm::vec3& cam_dir = glm::vec3(0.0f, 0.0f, -1.0f), const float& fov = glm::radians(90.0f), const float& near_plane = 0.1f, const float& far_plane = 10000.0f)
		: proj(proj), cam_pos(cam_pos), cam_dir(cam_dir), fov(fov), near_plane(near_plane), far_plane(far_plane)
	{}
	CameraComponent(CameraComponent&& o) = default;
	CameraComponent& operator=(CameraComponent&& o) = default;
	~CameraComponent() = default;
};

struct ScriptComponent
{
	Script* script = nullptr;

	ScriptComponent(Script* script)
		: script(script)
	{ assert(script); }
};

struct SpriteComponent
{
	GLuint tex_id;
#pragma warning( disable : 4201 )
	union
	{
		glm::vec4 data;
		struct {
		glm::vec2 texel_origin;
		glm::vec2 size;
		};
	};
	const Texture* tex;
	Renderer2D::SCREEN_ANCHOR anchor;

	SpriteComponent(const Texture& texture)
		: tex_id(texture.GetID()), data{ 0.0f, 0.0f, texture.GetWidth(), texture.GetHeight() }, tex(&texture), anchor(Renderer2D::SCREEN_ANCHOR::TOP_LEFT)
	{}
	SpriteComponent(const Texture& texture, const Renderer2D::SCREEN_ANCHOR& anchor)
		: tex_id(texture.GetID()), data{ 0.0f, 0.0f, texture.GetWidth(), texture.GetHeight() }, tex(&texture), anchor(anchor)
	{}
	SpriteComponent(const Texture& texture, const Renderer2D::SCREEN_ANCHOR& anchor, const glm::vec2& texel_origin, const glm::vec2& size)
		: tex_id(texture.GetID()), data{ texel_origin, size }, tex(&texture), anchor(anchor)
	{}
	SpriteComponent(const Texture& texture, const Renderer2D::SCREEN_ANCHOR& anchor, const float& texel_origin_x, const float& texel_origin_y, const float& size_x, const float& size_y)
		: tex_id(texture.GetID()), data{texel_origin_x, texel_origin_y, size_x, size_y}, tex(&texture), anchor(anchor)
	{}
	SpriteComponent(SpriteComponent&& o) = default;
	SpriteComponent& operator=(SpriteComponent&& o) = default;
};

struct MeshComponent
{
	GLuint vao;
	uint32_t num_indices;
	bool cast_shadow;

	MeshComponent(const Model* const mod, const bool& cast_shadow = true)
		: vao(mod->GetVAO()), num_indices(mod->GetNumTriangles() * 3), cast_shadow(cast_shadow)
	{}
	MeshComponent(MeshComponent&& o) = default;
	MeshComponent& operator=(MeshComponent&& o) = default;

private:
	uint8_t pad0 = 0;
	uint8_t pad1 = 0;
	uint8_t pad2 = 0;
};

struct MaterialComponent
{
	VertexTypes::PhongADS ads;
	glm::uvec4 tex_id;
	glm::uvec4 norm_tex_id;
	glm::vec4 col;

	MaterialComponent()
		: ads(), tex_id(TextureLoader::Get("default").GetID()), norm_tex_id(TextureLoader::Get("default_normal").GetID()), col(Colors::White) {}
	MaterialComponent(const VertexTypes::PhongADS& ads, const glm::uvec4& tex_id, const glm::uvec4& norm_tex_id, const glm::vec4& color = Colors::White)
		: ads(ads), tex_id(tex_id), norm_tex_id(norm_tex_id), col(color) {}
	MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint& tex0, const glm::vec4& color = Colors::White)
		: ads(ads), tex_id(tex0, TextureLoader::Get("default").GetID(), TextureLoader::Get("default").GetID(), TextureLoader::Get("default").GetID()), norm_tex_id(0), col(color) {}
	MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint& tex0, const GLuint& tex1, const glm::vec4& color = Colors::White)
		: ads(ads), tex_id(tex0, tex1, TextureLoader::Get("default").GetID(), TextureLoader::Get("default").GetID()), norm_tex_id(0), col(color) {}
	MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint& tex0, const GLuint& tex1, const GLuint& tex2, const glm::vec4& color = Colors::White)
		: ads(ads), tex_id(tex0, tex1, tex2, TextureLoader::Get("default").GetID()), norm_tex_id(0), col(color) {}
	MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint& tex0, const GLuint& tex1, const GLuint& tex2, const GLuint& tex3, const glm::vec4& color = Colors::White)
		: ads(ads), tex_id(tex0, tex1, tex2, tex3), norm_tex_id(0), col(color) {}
	MaterialComponent(MaterialComponent&& o) = default;
	MaterialComponent& operator=(MaterialComponent&& o) = default;

	void SetAmbientDiffuseSpecular(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float& shininess)
	{
		ads.ambient = ambient;
		ads.diffuse = diffuse;
		ads.specular = specular;
		ads.shininess = shininess;
	}
	void SetAmbientDiffuseSpecular(const VertexTypes::PhongADS& mat) 
	{ 
		ads = mat; 
	}
	void SetTexture(const uint32_t index, const Texture& tex) 
	{ 
		assert(index >= 0 && index < 4); 
		tex_id[index] = tex; 
	}
	void SetTextures(const Texture& tex0, const Texture& tex1, const Texture& tex2, const Texture& tex3)
	{
		tex_id.x = tex0;
		tex_id.y = tex1;
		tex_id.z = tex2;
		tex_id.w = tex3;
	}
	void SetColor(const glm::vec4& color) 
	{
		col = color;
	}
};

struct DirectionalLightComponent
{
	VertexTypes::DirectionalLight light;

	DirectionalLightComponent() = default;
	DirectionalLightComponent(const VertexTypes::DirectionalLight& dir_light)
		: light(dir_light)
	{}
	DirectionalLightComponent(const VertexTypes::PhongADS& ads, const glm::vec3& dir)
		: light(ads, dir)
	{}
	DirectionalLightComponent(const glm::vec3& dir)
		: light(VertexTypes::PhongADS(glm::vec3(.3f), glm::vec3(1.0f), glm::vec3(1.0f), 0), dir)
	{}
	DirectionalLightComponent(DirectionalLightComponent&& o) = default;
	DirectionalLightComponent& operator=(DirectionalLightComponent&& o) = default;
	~DirectionalLightComponent() = default;
};

struct SkyboxComponent
{
	GLuint tex_id = 0xFFFFFFF;

	SkyboxComponent() = default;
	SkyboxComponent(const GLuint& tex)
		: tex_id(tex)
	{}
};

#endif _COMPONENTS