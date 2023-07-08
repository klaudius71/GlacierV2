#ifndef _MESH_COMPONENT
#define _MESH_COMPONENT

#include "GlacierCore.h"

class Model;

#if GLACIER_OPENGL
struct GLACIER_API MeshComponent
{
	const Model* mod;
	GLuint vao;
	uint32_t num_indices;
	bool cast_shadow;

	MeshComponent(const Model* const mod, const bool cast_shadow = true);
	MeshComponent(MeshComponent&& o);
	MeshComponent& operator=(MeshComponent&& o);

private:
	uint8_t pad0 = 0;
	uint8_t pad1 = 0;
	uint8_t pad2 = 0;
};
#elif GLACIER_DIRECTX
struct GLACIER_API MeshComponent
{
	const Model* mod;
	bool cast_shadow;

	MeshComponent(const Model* const mod, const bool cast_shadow = true);
	MeshComponent(MeshComponent&& o);
	MeshComponent& operator=(MeshComponent&& o);

private:
	uint8_t pad0 = 0;
	uint8_t pad1 = 0;
	uint8_t pad2 = 0;
};
#endif

#endif