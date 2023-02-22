#ifndef _CHARACTER_CONTROLLER_COMPONENT
#define _CHARACTER_CONTROLLER_COMPONENT

#include "GlacierCore.h"

class btCollisionShape;
struct btDefaultMotionState;
class btRigidBody;

class btPairCachingGhostObject;
class btKinematicCharacterController;

class DynamicCharacterController;

struct GLACIER_API CharacterControllerComponent
{
	//btCollisionShape* shape;
	//btDefaultMotionState* motion_state;
	//btRigidBody* rb;

	//btConvexShape* m_convexShape;
	//btPairCachingGhostObject* m_ghostObject;
	//btKinematicCharacterController* m_controller;

	DynamicCharacterController* m_controller;

	CharacterControllerComponent(const float& radius = 10.0f, const float& height = 60.0f);

	CharacterControllerComponent(const CharacterControllerComponent&) = delete;
	CharacterControllerComponent& operator=(const CharacterControllerComponent&) = delete;
	CharacterControllerComponent(CharacterControllerComponent&& o);
	CharacterControllerComponent& operator=(CharacterControllerComponent&& o);
	~CharacterControllerComponent();

	// Will expand on these later
	//void Activate(bool force = false); // Apparently applying a force or impulse doesn't activate the rigidbody
	//void ApplyCentralImpulse(const glm::vec3& impulse);
	//void ApplyTorqueImpulse(const glm::vec3& torque);

	void AddMovement(const glm::vec3& direction);
	bool CanJump() const;
	void Jump() const;
};

#endif