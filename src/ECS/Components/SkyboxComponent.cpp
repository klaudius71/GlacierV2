#include "gpch.h"
#include "SkyboxComponent.h"

#if GLACIER_OPENGL
SkyboxComponent::SkyboxComponent(const GLuint tex)
	: tex_id(tex)
{}
#elif GLACIER_DIRECTX
SkyboxComponent::SkyboxComponent(const Texture& tex)
	: tex(&tex)
{}
#endif