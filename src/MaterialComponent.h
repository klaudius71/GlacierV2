#ifndef _MATERIAL_COMPONENT
#define _MATERIAL_COMPONENT

class Texture;

struct MaterialComponent
{
	VertexTypes::PhongADS ads;
	glm::uvec4 tex_id;
	glm::uvec4 norm_tex_id;
	glm::vec4 col;

	MaterialComponent();
	MaterialComponent(const VertexTypes::PhongADS& ads, const glm::uvec4& tex_id, const glm::uvec4& norm_tex_id, const glm::vec4& color = Colors::White);
	MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint& tex0, const glm::vec4& color = Colors::White);
	MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint& tex0, const GLuint& tex1, const glm::vec4& color = Colors::White);
	MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint& tex0, const GLuint& tex1, const GLuint& tex2, const glm::vec4& color = Colors::White);
	MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint& tex0, const GLuint& tex1, const GLuint& tex2, const GLuint& tex3, const glm::vec4& color = Colors::White);
	MaterialComponent(MaterialComponent&& o) = default;
	MaterialComponent& operator=(MaterialComponent&& o) = default;

	void SetAmbientDiffuseSpecular(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float& shininess);
	void SetAmbientDiffuseSpecular(const VertexTypes::PhongADS& mat);
	void SetTexture(const uint32_t index, const Texture& tex);
	void SetTextures(const Texture& tex0, const Texture& tex1, const Texture& tex2, const Texture& tex3);
	void SetColor(const glm::vec4& color);
};


#endif