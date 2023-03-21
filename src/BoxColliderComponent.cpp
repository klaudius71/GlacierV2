#include "gpch.h"
#include "BoxColliderComponent.h"
#include "Physics.h"

BoxColliderComponent::BoxColliderComponent(const glm::vec3& half_extents, float friction, float rolling_friction, float restitution, float mass)
	: RigidbodyComponent()
{
	shape = new btBoxShape(btVector3(half_extents.x, half_extents.y, half_extents.z));

	btVector3 inertia;
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, inertia);
	info.m_friction = friction;
	info.m_rollingFriction = rolling_friction;
	info.m_restitution = restitution;

	rb = new btRigidBody(info);
	Physics::AddRigidbodyToWorld(rb);
}
BoxColliderComponent::BoxColliderComponent(BoxColliderComponent&& o)
	: RigidbodyComponent(std::move(o))
{
}
BoxColliderComponent& BoxColliderComponent::operator=(BoxColliderComponent&& o)
{
	RigidbodyComponent::operator=(std::move(o));
	
	return *this;
}
