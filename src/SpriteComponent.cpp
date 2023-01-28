#include "gpch.h"
#include "SpriteComponent.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(const Texture& texture)
	: tex_id(texture.GetID()), data{ 0.0f, 0.0f, texture.GetWidth(), texture.GetHeight() }, tex(&texture), anchor(Renderer2D::SCREEN_ANCHOR::TOP_LEFT)
{}
SpriteComponent::SpriteComponent(const Texture& texture, const Renderer2D::SCREEN_ANCHOR& anchor)
	: tex_id(texture.GetID()), data{ 0.0f, 0.0f, texture.GetWidth(), texture.GetHeight() }, tex(&texture), anchor(anchor)
{}
SpriteComponent::SpriteComponent(const Texture& texture, const Renderer2D::SCREEN_ANCHOR& anchor, const glm::vec2& texel_origin, const glm::vec2& size)
	: tex_id(texture.GetID()), data{ texel_origin, size }, tex(&texture), anchor(anchor)
{}
SpriteComponent::SpriteComponent(const Texture& texture, const Renderer2D::SCREEN_ANCHOR& anchor, const float& texel_origin_x, const float& texel_origin_y, const float& size_x, const float& size_y)
	: tex_id(texture.GetID()), data{ texel_origin_x, texel_origin_y, size_x, size_y }, tex(&texture), anchor(anchor)
{}