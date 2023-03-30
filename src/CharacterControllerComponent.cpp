#include "gpch.h"
#include "Physics.h"
#include "CharacterControllerComponent.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "TimeManager.h"
#include "DynamicCharacterController.h"

//CharacterControllerComponent::CharacterControllerComponent(const float& radius, const float& height)
//	: m_ghostObject(new btPairCachingGhostObject()),
//	m_convexShape(new btCapsuleShape(radius, height)),
//	m_controller(new btKinematicCharacterController(m_ghostObject, m_convexShape, 1.0f))
//{
//	m_ghostObject->setCollisionShape(m_convexShape);
//	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT);
//	m_ghostObject->setWorldTransform(btTransform(btMatrix3x3::getIdentity(), btVector3(0.0f, 300.0f, 0.0f)));
//
//	//m_controller->setGravity(btVector3(0.0f, -98.1f, 0.0f));
//
//	Physics::AddCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
//	Physics::AddAction(m_controller);
//}
//
//CharacterControllerComponent::CharacterControllerComponent(CharacterControllerComponent&& o)
//	: m_convexShape(o.m_convexShape), m_ghostObject(o.m_ghostObject), m_controller(o.m_controller)
//{
//	o.m_convexShape = nullptr;
//	o.m_ghostObject = nullptr;
//	o.m_controller = nullptr;
//}
//CharacterControllerComponent& CharacterControllerComponent::operator=(CharacterControllerComponent&& o)
//{
//	m_convexShape = o.m_convexShape;
//	m_ghostObject = o.m_ghostObject;
//	m_controller = o.m_controller;
//
//	o.m_convexShape = nullptr;
//	o.m_ghostObject = nullptr;
//	o.m_controller = nullptr;
//
//	return *this;
//}
//CharacterControllerComponent::~CharacterControllerComponent()
//{
//	if (m_controller)
//	{
//		Physics::RemoveAction(m_controller);
//		Physics::RemoveCollisionObject(m_ghostObject);
//		delete m_controller;
//		delete m_ghostObject;
//		delete m_convexShape;
//	}
//}

CharacterControllerComponent::CharacterControllerComponent(const float radius, const float height)
	: m_controller(new DynamicCharacterController(height, radius))
{
	m_controller->AddToScene();
}
CharacterControllerComponent::CharacterControllerComponent(CharacterControllerComponent&& o)
	: m_controller(o.m_controller)
{
	o.m_controller = nullptr;
}
CharacterControllerComponent& CharacterControllerComponent::operator=(CharacterControllerComponent&& o)
{
	m_controller = o.m_controller;

	o.m_controller = nullptr;

	return *this;
}
CharacterControllerComponent::~CharacterControllerComponent()
{
	if (m_controller)
	{
		m_controller->RemoveFromScene();
		delete m_controller;
	}
}

void CharacterControllerComponent::AddMovement(const glm::vec3& direction)
{
	assert(m_controller);
	m_controller->SetWalkDirection(direction);
}

bool CharacterControllerComponent::CanJump() const
{
	assert(m_controller);
	return m_controller->canJump();
}
void CharacterControllerComponent::Jump() const
{
	assert(m_controller);
	m_controller->jump();
}

//CharacterControllerComponent::CharacterControllerComponent(const float& radius, const float& height)
//	: shape(new btCapsuleShape(radius, height)),
//	motion_state(new btDefaultMotionState)
//{
//	btVector3 inertia;
//	shape->calculateLocalInertia(100.0f, inertia);
//	btRigidBody::btRigidBodyConstructionInfo info(100.0f, motion_state, shape, inertia);
//	info.m_friction = .1f;
//	info.m_rollingFriction = .1f;
//	info.m_restitution = .5f;
//
//	rb = new btRigidBody(info);
//	rb->setAngularFactor(0.0f);
//	Physics::AddRigidbodyToWorld(rb);
//}
//CharacterControllerComponent::CharacterControllerComponent(CharacterControllerComponent&& o)
//	: shape(o.shape), motion_state(o.motion_state), rb(o.rb)
//{
//	o.shape = nullptr;
//	o.motion_state = nullptr;
//	o.rb = nullptr;
//}
//CharacterControllerComponent& CharacterControllerComponent::operator=(CharacterControllerComponent&& o)
//{
//	shape = o.shape;
//	motion_state = o.motion_state;
//	rb = o.rb;
//
//	o.shape = nullptr;
//	o.motion_state = nullptr;
//	o.rb = nullptr;
//
//	return *this;
//}
//CharacterControllerComponent::~CharacterControllerComponent()
//{
//	if (rb)
//	{
//		Physics::RemoveRigidbodyFromWorld(rb);
//		delete shape;
//		delete rb;
//		delete motion_state;
//	}
//}

//void CharacterControllerComponent::Activate(bool force)
//{
//	assert(rb);
//	rb->activate(force);
//}
//void CharacterControllerComponent::ApplyCentralImpulse(const glm::vec3& impulse)
//{
//	assert(rb);
//	Activate();
//	rb->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
//}
//void CharacterControllerComponent::ApplyTorqueImpulse(const glm::vec3& torque)
//{
//	assert(rb);
//	Activate();
//	rb->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
//}