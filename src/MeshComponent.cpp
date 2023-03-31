#include "gpch.h"
#include "MeshComponent.h"
#include "ModelOpenGL.h"

MeshComponent::MeshComponent(const ModelOpenGL* const mod, const bool cast_shadow)
	: mod(mod), vao(mod->GetVAO()), num_indices(mod->GetNumTriangles() * 3), cast_shadow(cast_shadow)
{}
MeshComponent::MeshComponent(MeshComponent&& o)
	: mod(o.mod), vao(o.vao), num_indices(o.num_indices), cast_shadow(o.cast_shadow)
{
	o.mod = nullptr;
	o.vao = 0;
}
MeshComponent& MeshComponent::operator=(MeshComponent&& o)
{
	mod = o.mod;
	vao = o.vao;
	num_indices = o.num_indices;
	cast_shadow = o.cast_shadow;

	o.mod = nullptr;
	o.vao = 0;

	return *this;
}