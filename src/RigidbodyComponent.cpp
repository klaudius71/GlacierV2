#include "gpch.h"
#include "RigidbodyComponent.h"
#include "Model.h"

RigidbodyComponent::RigidbodyComponent(PHYSICS_PLANE, const glm::vec3& normal)
	: shape(new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), 0.0f)),
	motion_state(new btDefaultMotionState),
	mesh(nullptr)
{
	btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion_state, shape);
	info.m_friction = 10.0f;
	info.m_rollingFriction = 10.0f;
	info.m_restitution = 0.5f;

	rb = new btRigidBody(info);
	Physics::AddRigidbodyToWorld(rb);
}
RigidbodyComponent::RigidbodyComponent(PHYSICS_BOX, const float& half_extent_x, const float& half_extent_y, const float& half_extent_z, const float& mass)
	: shape(new btBoxShape(btVector3(half_extent_x, half_extent_y, half_extent_z))),
	motion_state(new btDefaultMotionState),
	mesh(nullptr)
{
	btVector3 inertia;
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, inertia);
	info.m_friction = .6f;
	info.m_rollingFriction = .4f;
	info.m_restitution = 0.5f;

	rb = new btRigidBody(info);
	Physics::AddRigidbodyToWorld(rb);
}
RigidbodyComponent::RigidbodyComponent(PHYSICS_SPHERE, const float& radius, const float& mass)
	: shape(new btSphereShape(radius)),
	motion_state(new btDefaultMotionState),
	mesh(nullptr)
{
	btVector3 inertia;
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, inertia);
	info.m_friction = 1.0f;
	info.m_rollingFriction = .9f;
	info.m_restitution = .5f;

	rb = new btRigidBody(info);
	Physics::AddRigidbodyToWorld(rb);
}
RigidbodyComponent::RigidbodyComponent(const Model* mod)
	: motion_state(new btDefaultMotionState)
{
	GLACIER_DEBUG_FUNC_TIMER("RigidbodyComponent(const Model* mod)... ");
	mesh = new btTriangleIndexVertexArray(mod->GetNumTriangles(), (int*)mod->GetTriangles().data(), sizeof(VertexTypes::VertexTriangle),
		mod->GetNumVertices(), (float*)mod->GetVertexData().data(), sizeof(VertexTypes::Vertex));
	shape = new btBvhTriangleMeshShape(mesh, true);

	btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion_state, shape);
	info.m_friction = 10.0f;
	info.m_rollingFriction = 10.0f;
	info.m_restitution = 0.5f;

	rb = new btRigidBody(info);
	Physics::AddRigidbodyToWorld(rb);
}
RigidbodyComponent::RigidbodyComponent(RigidbodyComponent&& o)
	: rb(o.rb), motion_state(o.motion_state), shape(o.shape), mesh(o.mesh)
{
	o.rb = nullptr;
	o.motion_state = nullptr;
	o.shape = nullptr;
	o.mesh = nullptr;
}
RigidbodyComponent& RigidbodyComponent::operator=(RigidbodyComponent&& o)
{
	rb = o.rb;
	motion_state = o.motion_state;
	shape = o.shape;
	mesh = o.mesh;

	o.rb = nullptr;
	o.motion_state = nullptr;
	o.shape = nullptr;
	o.mesh = nullptr;

	return *this;
}
RigidbodyComponent::~RigidbodyComponent()
{
	if (rb)
	{
		Physics::RemoveRigidbodyFromWorld(rb);
		delete shape;
		delete rb;
		delete motion_state;
		delete mesh;
	}
}

void RigidbodyComponent::Activate(bool force)
{
	assert(rb);
	rb->activate(force);
}
void RigidbodyComponent::ApplyCentralImpulse(const glm::vec3& impulse)
{
	assert(rb);
	rb->activate(); // Apparently applying a force or impulse doesn't activate the rigidbody
	rb->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}
void RigidbodyComponent::ApplyTorqueImpulse(const glm::vec3& torque)
{
	assert(rb);
	rb->activate();
	rb->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
}
