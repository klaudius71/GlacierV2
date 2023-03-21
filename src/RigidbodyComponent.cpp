#include "gpch.h"
#include "RigidbodyComponent.h"
#include "Physics.h"
#include "Model.h"

RigidbodyComponent::RigidbodyComponent()
	: shape(nullptr), motion_state(new btDefaultMotionState), rb(nullptr)
{
}
RigidbodyComponent::RigidbodyComponent(RigidbodyComponent&& o)
	: rb(o.rb), motion_state(o.motion_state), shape(o.shape)
{
	o.rb = nullptr;
	o.motion_state = nullptr;
	o.shape = nullptr;
}
RigidbodyComponent& RigidbodyComponent::operator=(RigidbodyComponent&& o)
{
	rb = o.rb;
	motion_state = o.motion_state;
	shape = o.shape;

	o.rb = nullptr;
	o.motion_state = nullptr;
	o.shape = nullptr;

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
	}
}

void RigidbodyComponent::Activate(bool force)
{
	assert(rb);
	rb->activate(force);
}
void RigidbodyComponent::SetWorldPosition(const glm::vec3& pos)
{
	assert(rb);
	rb->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
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