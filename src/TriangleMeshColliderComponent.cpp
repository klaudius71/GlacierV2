#include "gpch.h"
#include "TriangleMeshColliderComponent.h"
#include "Model.h"
#include "Physics.h"

TriangleMeshColliderComponent::TriangleMeshColliderComponent(const Model* model, float friction, float rolling_friction, float restitution)
	: RigidbodyComponent()
{
	GLACIER_DEBUG_FUNC_TIMER("RigidbodyComponent(const Model* mod)... ");
	mesh = new btTriangleIndexVertexArray(model->GetNumTriangles(), (int*)model->GetTriangles().data(), sizeof(VertexTypes::VertexTriangle),
											model->GetNumVertices(), (float*)model->GetVertexData().data(), sizeof(VertexTypes::Vertex));
	shape = new btBvhTriangleMeshShape(mesh, true);

	btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion_state, shape);
	info.m_friction = friction;
	info.m_rollingFriction = rolling_friction;
	info.m_restitution = restitution;

	rb = new btRigidBody(info);
	Physics::AddRigidbodyToWorld(rb);
}
TriangleMeshColliderComponent::TriangleMeshColliderComponent(TriangleMeshColliderComponent&& o)
	: RigidbodyComponent(std::move(o)), mesh(o.mesh)
{
	o.mesh = nullptr;
}
TriangleMeshColliderComponent& TriangleMeshColliderComponent::operator=(TriangleMeshColliderComponent&& o)
{
	mesh = o.mesh;
	RigidbodyComponent::operator=(std::move(o));

	o.mesh = nullptr;

	return *this;
}
TriangleMeshColliderComponent::~TriangleMeshColliderComponent()
{
	delete mesh;
}