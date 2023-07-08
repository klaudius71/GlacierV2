#include "gpch.h"
#include "SphereColliderComponent.h"
#include "Physics.h"

SphereColliderComponent::SphereColliderComponent(float radius, float mass, float rolling_friction, float friction, float restitution)
	: RigidbodyComponent()
{
	shape = new btSphereShape(radius);

	btVector3 inertia;
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, inertia);
	info.m_friction = friction;
	info.m_rollingFriction = rolling_friction;
	info.m_restitution = restitution;

	rb = new btRigidBody(info);
	Physics::AddRigidbodyToWorld(rb);
}
SphereColliderComponent::SphereColliderComponent(SphereColliderComponent&& o)
	: RigidbodyComponent(std::move(o))
{
}

SphereColliderComponent& SphereColliderComponent::operator=(SphereColliderComponent&& o)
{
	RigidbodyComponent::operator=(std::move(o));

	return *this;
}
