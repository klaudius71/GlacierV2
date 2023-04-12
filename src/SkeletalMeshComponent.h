#ifndef _SKELETAL_MESH_COMPONENT
#define _SKELETAL_MESH_COMPONENT

#include "GlacierCore.h"

#define MAX_BONES 100

class Model;

#if GLACIER_OPENGL
class ModelOpenGL;

struct GLACIER_API SkeletalMeshComponent
{
	const ModelOpenGL* mod;
	GLuint vao;
	uint32_t num_indices;
	uint32_t num_joints;
	glm::mat4* bone_matrices;
	bool cast_shadow;

	SkeletalMeshComponent(const Model* const mod, const bool cast_shadow = true);
	SkeletalMeshComponent(const SkeletalMeshComponent& o) = delete;
	SkeletalMeshComponent& operator=(const SkeletalMeshComponent&) = delete;
	SkeletalMeshComponent(SkeletalMeshComponent&& o);
	SkeletalMeshComponent& operator=(SkeletalMeshComponent&& o);
	~SkeletalMeshComponent();

private:
	uint8_t pad0 = 0;
	uint8_t pad1 = 0;
	uint8_t pad2 = 0;
};
#elif GLACIER_DIRECTX
struct GLACIER_API SkeletalMeshComponent
{
	const Model* mod;
	uint32_t num_joints;
	glm::mat4* bone_matrices;
	bool cast_shadow;

	SkeletalMeshComponent(const Model* const mod, const bool cast_shadow = true);
	SkeletalMeshComponent(const SkeletalMeshComponent& o) = delete;
	SkeletalMeshComponent& operator=(const SkeletalMeshComponent&) = delete;
	SkeletalMeshComponent(SkeletalMeshComponent&& o);
	SkeletalMeshComponent& operator=(SkeletalMeshComponent&& o);
	~SkeletalMeshComponent();

private:
	uint8_t pad0 = 0;
	uint8_t pad1 = 0;
	uint8_t pad2 = 0;
};
#endif

#endif