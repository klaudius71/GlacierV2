#include "gpch.h"
#include "SpriteComponent.h"

#if GLACIER_OPENGL
#include "TextureOpenGL.h"

SpriteComponent::SpriteComponent(const TextureOpenGL& texture)
	: tex_id(texture.GetID()), data{ 0.0f, 0.0f, texture.GetWidth(), texture.GetHeight() }, tex(&texture), anchor(Renderer2D::SCREEN_ANCHOR::TOP_LEFT)
{}
SpriteComponent::SpriteComponent(const TextureOpenGL& texture, const Renderer2D::SCREEN_ANCHOR anchor)
	: tex_id(texture.GetID()), data{ 0.0f, 0.0f, texture.GetWidth(), texture.GetHeight() }, tex(&texture), anchor(anchor)
{}
SpriteComponent::SpriteComponent(const TextureOpenGL& texture, const Renderer2D::SCREEN_ANCHOR anchor, const glm::vec2 texel_origin, const glm::vec2 size)
	: tex_id(texture.GetID()), data{ texel_origin, size }, tex(&texture), anchor(anchor)
{}
SpriteComponent::SpriteComponent(const TextureOpenGL& texture, const Renderer2D::SCREEN_ANCHOR anchor, const float texel_origin_x, const float texel_origin_y, const float size_x, const float size_y)
	: tex_id(texture.GetID()), data{ texel_origin_x, texel_origin_y, size_x, size_y }, tex(&texture), anchor(anchor)
{}
#elif GLACIER_DIRECTX
#include "TextureDirectX.h"
SpriteComponent::SpriteComponent(const TextureDirectX& texture)
	: data{ 0.0f, 0.0f, texture.GetWidth(), texture.GetHeight() }, tex(&texture), anchor(Renderer2D::SCREEN_ANCHOR::TOP_LEFT)
{}
SpriteComponent::SpriteComponent(const TextureDirectX& texture, const Renderer2D::SCREEN_ANCHOR anchor)
	: data{ 0.0f, 0.0f, texture.GetWidth(), texture.GetHeight() }, tex(&texture), anchor(anchor)
{}
SpriteComponent::SpriteComponent(const TextureDirectX& texture, const Renderer2D::SCREEN_ANCHOR anchor, const glm::vec2 texel_origin, const glm::vec2 size)
	: data{ texel_origin, size }, tex(&texture), anchor(anchor)
{}
SpriteComponent::SpriteComponent(const TextureDirectX& texture, const Renderer2D::SCREEN_ANCHOR anchor, const float texel_origin_x, const float texel_origin_y, const float size_x, const float size_y)
	: data{ texel_origin_x, texel_origin_y, size_x, size_y }, tex(&texture), anchor(anchor)
{}
#endif