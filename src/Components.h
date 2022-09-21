#ifndef _COMPONENTS
#define _COMPONENTS

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
	{}
	NameComponent(std::string&& name)
		: id(std::hash<std::string>()(name)), name(std::move(name))
	{}
	NameComponent(NameComponent&& o)
		: id(o.id), name(std::move(o.name))
	{}
	NameComponent& operator=(NameComponent&& o)
	{ name = std::move(o.name); return *this; }
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
public:
	bool flag_changed = true;
private:
	uint8_t pad0 = 0;
	uint8_t pad1 = 0;
	uint8_t pad2 = 0;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 rot = glm::vec3(0.0f);
	glm::vec3 scl = glm::vec3(1.0f);

	friend class Scene;
	friend class GameObject;
};

struct CameraComponent
{
	glm::mat4 proj = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	//glm::vec4 cam_pos = glm::vec4(0.0f);

	CameraComponent() = default;
	CameraComponent(const glm::mat4& proj, const glm::mat4& view/*, const glm::vec4& cam_pos*/)
		: proj(proj), view(view)//, cam_pos(cam_pos)
	{}
	CameraComponent(const CameraComponent&) = default;
	CameraComponent& operator=(const CameraComponent&) = default;
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

struct Render2DComponent
{
	GLuint shad;
	GLuint tex_id;

	Render2DComponent(const Shader* const shad, const Texture* texture)
		: shad(shad->GetProgramID()), tex_id(texture->GetID())
	{}
	Render2DComponent(Render2DComponent&& o)
		: shad(o.shad), tex_id(o.tex_id)
	{}
	Render2DComponent& operator=(Render2DComponent&& o)
	{
		shad = o.shad;
		tex_id = o.tex_id;
		return *this;
	}
};

struct MeshComponent
{
	GLuint vao;
	uint32_t num_indices;

	MeshComponent(const Model* const mod)
		: vao(mod->GetVAO()), num_indices(mod->GetNumTriangles() * 3)
	{}
	MeshComponent(MeshComponent&& o)
		: vao(o.vao), num_indices(o.num_indices)
	{}
	MeshComponent& operator=(MeshComponent&& o)
	{
		vao = o.vao;
		num_indices = o.num_indices;
		return *this;
	}
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
	MaterialComponent(MaterialComponent&& o)
		: ads(o.ads), tex_id(o.tex_id), norm_tex_id(o.norm_tex_id), col(o.col) {}
	MaterialComponent& operator=(MaterialComponent&& o)
	{
		ads = o.ads;
		tex_id = o.tex_id;
		norm_tex_id = o.norm_tex_id;
		col = o.col;
		return *this;
	}

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
	DirectionalLightComponent(DirectionalLightComponent&& o)
		: light(o.light)
	{ o.light = VertexTypes::DirectionalLight(); }
	DirectionalLightComponent& operator=(DirectionalLightComponent&& o)
	{
		light = o.light;
		o.light = VertexTypes::DirectionalLight();
		return *this;
	}
	~DirectionalLightComponent() = default;
};

#endif _COMPONENTS