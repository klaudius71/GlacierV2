#include "gpch.h"
#include "BSphereTestingScript.h"

BSphereTestingScript::BSphereTestingScript(GameObject mesh)
	: mesh(mesh)
{
	assert(mesh->HasComponent<SkeletalMeshComponent>());
}

void BSphereTestingScript::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	TransformComponent& transform = GetGameObject()->GetComponent<TransformComponent>();
	SkeletalMeshComponent& mesh_component = mesh->GetComponent<SkeletalMeshComponent>();
	TransformComponent& mesh_transform = mesh->GetComponent<TransformComponent>();
	transform.position() = mesh_transform.GetWorldMatrix() * glm::vec4(mesh_component.mod->GetBSphereCenter(), 1.0f);
	transform.scale() = glm::vec3(glm::length(mesh_transform.GetWorldMatrix()[0]),
								  glm::length(mesh_transform.GetWorldMatrix()[1]),
								  glm::length(mesh_transform.GetWorldMatrix()[2])) * mesh_component.mod->GetBSphereRadius();
}