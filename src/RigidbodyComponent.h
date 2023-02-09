#ifndef _RIGIDBODY_COMPONENT
#define _RIGIDBODY_COMPONENT

#include "GlacierCore.h"

#include "Physics.h"

class Model;

struct GLACIER_API RigidbodyComponent
{
	btCollisionShape* shape;
	btDefaultMotionState* motion_state;
	btRigidBody* rb;
	btStridingMeshInterface* mesh;

	RigidbodyComponent(PHYSICS_PLANE, const glm::vec3& normal = glm::vec3(0.0f, 1.0f, 0.0f));
	RigidbodyComponent(PHYSICS_BOX, const float& half_extent_x, const float& half_extent_y, const float& half_extent_z, const float& mass = 100.0f);
	RigidbodyComponent(PHYSICS_SPHERE, const float& radius, const float& mass = 500.0f);
	RigidbodyComponent(const Model* mod);
	RigidbodyComponent(RigidbodyComponent&& o);
	RigidbodyComponent& operator=(RigidbodyComponent&& o);
	~RigidbodyComponent();

	// Will expand on these later
	void Activate(bool force = false); // Apparently applying a force or impulse doesn't activate the rigidbody
	void ApplyCentralImpulse(const glm::vec3& impulse);
	void ApplyTorqueImpulse(const glm::vec3& torque);
};

#endif