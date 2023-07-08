#ifndef _SKYBOX_COMPONENT
#define _SKYBOX_COMPONENT

#include "GlacierCore.h"

#if GLACIER_OPENGL
struct GLACIER_API SkyboxComponent
{
	GLuint tex_id = 0xFFFFFFF;

	SkyboxComponent() = default;
	SkyboxComponent(const GLuint tex);
};
#elif GLACIER_DIRECTX
class Texture;

struct GLACIER_API SkyboxComponent
{
	const Texture* tex;

	SkyboxComponent() = default;
	SkyboxComponent(const Texture& tex);
};
#endif

#endif