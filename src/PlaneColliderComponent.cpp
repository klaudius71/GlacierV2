#include "gpch.h"
#include "PlaneColliderComponent.h"
#include "Physics.h"

PlaneColliderComponent::PlaneColliderComponent(float friction, float rolling_friction, float restitution, float y_offset, const glm::vec3& normal)
	: RigidbodyComponent()
{
	shape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), 0.0f);
	motion_state->setWorldTransform(btTransform(btMatrix3x3::getIdentity(), btVector3(0.0f, y_offset, 0.0f)));

	btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion_state, shape);
	info.m_friction = friction;
	info.m_rollingFriction = rolling_friction;
	info.m_restitution = restitution;

	rb = new btRigidBody(info);
	Physics::AddRigidbodyToWorld(rb);
}
PlaneColliderComponent::PlaneColliderComponent(PlaneColliderComponent&& o)
	: RigidbodyComponent(std::move(o))
{
}
PlaneColliderComponent& PlaneColliderComponent::operator=(PlaneColliderComponent&& o)
{
	RigidbodyComponent::operator=(std::move(o));

	return *this;
}
