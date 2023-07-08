#ifndef _SPRITE_COMPONENT
#define _SPRITE_COMPONENT

#include "GlacierCore.h"
#include "Renderer2D.h"

#if GLACIER_OPENGL
class TextureOpenGL;

struct GLACIER_API SpriteComponent
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
	const TextureOpenGL* tex;
	Renderer2D::SCREEN_ANCHOR anchor;

	SpriteComponent(const TextureOpenGL& texture);
	SpriteComponent(const TextureOpenGL& texture, const Renderer2D::SCREEN_ANCHOR anchor);
	SpriteComponent(const TextureOpenGL& texture, const Renderer2D::SCREEN_ANCHOR anchor, const glm::vec2 texel_origin, const glm::vec2 size);
	SpriteComponent(const TextureOpenGL& texture, const Renderer2D::SCREEN_ANCHOR anchor, const float texel_origin_x, const float texel_origin_y, const float size_x, const float size_y);
	SpriteComponent(SpriteComponent&& o) = default;
	SpriteComponent& operator=(SpriteComponent&& o) = default;
};
#elif GLACIER_DIRECTX
class TextureDirectX;

struct GLACIER_API SpriteComponent
{
#pragma warning( disable : 4201 )
	union
	{
		glm::vec4 data;
		struct {
			glm::vec2 texel_origin;
			glm::vec2 size;
		};
	};
	const TextureDirectX* tex;
	Renderer2D::SCREEN_ANCHOR anchor;

	SpriteComponent(const TextureDirectX& texture);
	SpriteComponent(const TextureDirectX& texture, const Renderer2D::SCREEN_ANCHOR anchor);
	SpriteComponent(const TextureDirectX& texture, const Renderer2D::SCREEN_ANCHOR anchor, const glm::vec2 texel_origin, const glm::vec2 size);
	SpriteComponent(const TextureDirectX& texture, const Renderer2D::SCREEN_ANCHOR anchor, const float texel_origin_x, const float texel_origin_y, const float size_x, const float size_y);
	SpriteComponent(SpriteComponent&& o) = default;
	SpriteComponent& operator=(SpriteComponent&& o) = default;
};
#endif

#endif // !_SPRITE_COMPONENT
