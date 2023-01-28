#ifndef _SKYBOX_COMPONENT
#define _SKYBOX_COMPONENT

struct SkyboxComponent
{
	GLuint tex_id = 0xFFFFFFF;

	SkyboxComponent() = default;
	SkyboxComponent(const GLuint& tex);
};

#endif