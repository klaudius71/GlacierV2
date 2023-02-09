#include "gpch.h"
#include "Physics.h"
#include "CharacterControllerComponent.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

CharacterControllerComponent::CharacterControllerComponent(const float& radius, const float& height)
	: shape(new btCapsuleShape(radius, height)),
	motion_state(new btDefaultMotionState)
{
	btVector3 inertia;
	shape->calculateLocalInertia(100.0f, inertia);
	btRigidBody::btRigidBodyConstructionInfo info(100.0f, motion_state, shape, inertia);
	info.m_friction = .1f;
	info.m_rollingFriction = .1f;
	info.m_restitution = .5f;

	rb = new btRigidBody(info);
	rb->setAngularFactor(0.0f);
	Physics::AddRigidbodyToWorld(rb);
}
CharacterControllerComponent::CharacterControllerComponent(CharacterControllerComponent&& o)
	: shape(o.shape), motion_state(o.motion_state), rb(o.rb)
{
	o.shape = nullptr;
	o.motion_state = nullptr;
	o.rb = nullptr;
}
CharacterControllerComponent& CharacterControllerComponent::operator=(CharacterControllerComponent&& o)
{
	shape = o.shape;
	motion_state = o.motion_state;
	rb = o.rb;

	o.shape = nullptr;
	o.motion_state = nullptr;
	o.rb = nullptr;

	return *this;
}
CharacterControllerComponent::~CharacterControllerComponent()
{
	if (rb)
	{
		Physics::RemoveRigidbodyFromWorld(rb);
		delete shape;
		delete rb;
		delete motion_state;
	}
}

void CharacterControllerComponent::Activate(bool force)
{
	assert(rb);
	rb->activate(force);
}
void CharacterControllerComponent::ApplyCentralImpulse(const glm::vec3& impulse)
{
	assert(rb);
	Activate();
	rb->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}
void CharacterControllerComponent::ApplyTorqueImpulse(const glm::vec3& torque)
{
	assert(rb);
	Activate();
	rb->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
}