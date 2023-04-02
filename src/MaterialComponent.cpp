#include "gpch.h"
#include "MaterialComponent.h"
#include "TextureLoader.h"
#include "TextureOpenGL.h"

#if GLACIER_OPENGL
MaterialComponent::MaterialComponent()
	: ads(), tex_id(TextureLoader::Get("default").GetID()), norm_tex_id(TextureLoader::Get("default_normal")), col(Colors::White) {}
MaterialComponent::MaterialComponent(const VertexTypes::PhongADS& ads, const glm::uvec4& tex_id, const glm::uvec4& norm_tex_id, const glm::vec4& color)
	: ads(ads), tex_id(tex_id), norm_tex_id(norm_tex_id), col(color) {}
MaterialComponent::MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint tex0, const glm::vec4& color)
	: ads(ads), tex_id(tex0, glm::uvec3(TextureLoader::Get(PRELOADED_TEXTURES::DEFAULT))), norm_tex_id(TextureLoader::Get(PRELOADED_TEXTURES::NORMAL_DEFAULT)), col(color) {}
MaterialComponent::MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint tex0, const GLuint tex1, const glm::vec4& color)
	: ads(ads), tex_id(tex0, tex1, (GLuint)TextureLoader::Get(PRELOADED_TEXTURES::DEFAULT), TextureLoader::Get(PRELOADED_TEXTURES::DEFAULT).GetID()), norm_tex_id(TextureLoader::Get(PRELOADED_TEXTURES::NORMAL_DEFAULT)), col(color) {}
MaterialComponent::MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint tex0, const GLuint tex1, const GLuint tex2, const glm::vec4& color)
	: ads(ads), tex_id(tex0, tex1, tex2, (GLuint)TextureLoader::Get(PRELOADED_TEXTURES::DEFAULT)), norm_tex_id(TextureLoader::Get(PRELOADED_TEXTURES::NORMAL_DEFAULT)), col(color) {}
MaterialComponent::MaterialComponent(const VertexTypes::PhongADS& ads, const GLuint tex0, const GLuint tex1, const GLuint tex2, const GLuint tex3, const glm::vec4& color)
	: ads(ads), tex_id(tex0, tex1, tex2, tex3), norm_tex_id(TextureLoader::Get(PRELOADED_TEXTURES::NORMAL_DEFAULT)), col(color) {}

void MaterialComponent::SetAmbientDiffuseSpecular(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, const float& shininess)
{
	ads.ambient = ambient;
	ads.diffuse = diffuse;
	ads.specular = specular;
	ads.shininess = shininess;
}
void MaterialComponent::SetAmbientDiffuseSpecular(const VertexTypes::PhongADS & mat)
{
	ads = mat;
}
void MaterialComponent::SetTexture(const uint32_t index, const TextureOpenGL& tex)
{
	assert(index >= 0 && index < 4);
	tex_id[index] = tex;
}
void MaterialComponent::SetTextures(const TextureOpenGL& tex0, const TextureOpenGL& tex1, const TextureOpenGL& tex2, const TextureOpenGL& tex3)
{
	tex_id.x = tex0;
	tex_id.y = tex1;
	tex_id.z = tex2;
	tex_id.w = tex3;
}
void MaterialComponent::SetColor(const glm::vec4& color)
{
	col = color;
}
#endif