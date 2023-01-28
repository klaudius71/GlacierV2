#ifndef _SPRITE_COMPONENT
#define _SPRITE_COMPONENT

#include "Renderer2D.h"

class Texture;

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

	SpriteComponent(const Texture& texture);
	SpriteComponent(const Texture& texture, const Renderer2D::SCREEN_ANCHOR& anchor);
	SpriteComponent(const Texture& texture, const Renderer2D::SCREEN_ANCHOR& anchor, const glm::vec2& texel_origin, const glm::vec2& size);
	SpriteComponent(const Texture& texture, const Renderer2D::SCREEN_ANCHOR& anchor, const float& texel_origin_x, const float& texel_origin_y, const float& size_x, const float& size_y);
	SpriteComponent(SpriteComponent&& o) = default;
	SpriteComponent& operator=(SpriteComponent&& o) = default;
};

#endif // !_SPRITE_COMPONENT
