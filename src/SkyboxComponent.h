#ifndef _SKYBOX_COMPONENT
#define _SKYBOX_COMPONENT

#include "GlacierCore.h"

struct GLACIER_API SkyboxComponent
{
	GLuint tex_id = 0xFFFFFFF;

	SkyboxComponent() = default;
	SkyboxComponent(const GLuint& tex);
};

#endif