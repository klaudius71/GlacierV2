#ifndef _MESH_COMPONENT
#define _MESH_COMPONENT

#include "GlacierCore.h"

class ModelOpenGL;

struct GLACIER_API MeshComponent
{
	const ModelOpenGL* mod;
	GLuint vao;
	uint32_t num_indices;
	bool cast_shadow;

	MeshComponent(const ModelOpenGL* const mod, const bool cast_shadow = true);
	MeshComponent(MeshComponent&& o);
	MeshComponent& operator=(MeshComponent&& o);

private:
	uint8_t pad0 = 0;
	uint8_t pad1 = 0;
	uint8_t pad2 = 0;
};

#endif