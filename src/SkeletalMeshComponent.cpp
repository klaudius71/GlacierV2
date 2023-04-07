#include "gpch.h"
#include "SkeletalMeshComponent.h"
#include "ModelOpenGL.h"

#if GLACIER_OPENGL
SkeletalMeshComponent::SkeletalMeshComponent(const ModelOpenGL* const mod, const bool cast_shadow)
	: mod(mod), vao(mod->GetVAO()), num_indices(mod->GetNumTriangles() * 3), num_joints(mod->GetNumBones()),
	bone_matrices(new glm::mat4[MAX_BONES]),
	cast_shadow(cast_shadow)
{
	for (size_t i = 0; i < MAX_BONES; i++)
		bone_matrices[i] = glm::mat4(1.0f);
}
SkeletalMeshComponent::SkeletalMeshComponent(SkeletalMeshComponent&& o)
	: mod(o.mod), vao(o.vao), num_indices(o.num_indices), num_joints(o.num_joints),
	bone_matrices(o.bone_matrices),
	cast_shadow(o.cast_shadow)
{
	o.mod = nullptr;
	o.vao = 0;
	o.bone_matrices = nullptr;
}
SkeletalMeshComponent& SkeletalMeshComponent::operator=(SkeletalMeshComponent&& o)
{
	mod = o.mod;
	vao = o.vao;
	num_indices = o.num_indices;
	num_joints = o.num_joints;
	bone_matrices = o.bone_matrices;
	cast_shadow = o.cast_shadow;

	o.mod = nullptr;
	o.vao = 0;
	o.bone_matrices = nullptr;

	return *this;
}
SkeletalMeshComponent::~SkeletalMeshComponent()
{
	delete[] bone_matrices;
}
#elif GLACIER_DIRECTX
SkeletalMeshComponent::SkeletalMeshComponent(const Model* const mod, const bool cast_shadow)
	: mod(mod), num_joints(mod->GetNumBones()), 
	bone_matrices(new glm::mat4[MAX_BONES]),
	cast_shadow(cast_shadow)
{
	for (size_t i = 0; i < MAX_BONES; i++)
		bone_matrices[i] = glm::mat4(1.0f);
}
SkeletalMeshComponent::SkeletalMeshComponent(SkeletalMeshComponent&& o)
	: mod(o.mod), num_joints(o.num_joints),
	bone_matrices(o.bone_matrices),
	cast_shadow(o.cast_shadow)
{
	o.mod = nullptr;
	o.bone_matrices = nullptr;
}
SkeletalMeshComponent& SkeletalMeshComponent::operator=(SkeletalMeshComponent&& o)
{
	mod = o.mod;
	num_joints = o.num_joints;
	bone_matrices = o.bone_matrices;
	cast_shadow = o.cast_shadow;

	o.mod = nullptr;
	o.bone_matrices = nullptr;

	return *this;
}
SkeletalMeshComponent::~SkeletalMeshComponent()
{
	delete[] bone_matrices;
}
#endif