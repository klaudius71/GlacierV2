#ifndef _RIGIDBODY_COMPONENT
#define _RIGIDBODY_COMPONENT

#include "GlacierCore.h"

class btCollisionShape;
struct btDefaultMotionState;
class btRigidBody;

struct GLACIER_API RigidbodyComponent
{
	btCollisionShape* shape;
	btDefaultMotionState* motion_state;
	btRigidBody* rb;

protected:
	RigidbodyComponent();
	RigidbodyComponent(RigidbodyComponent&& o);
	RigidbodyComponent& operator=(RigidbodyComponent&& o);
	virtual ~RigidbodyComponent();

public:
	// Will expand on these later
	void Activate(bool force = false); // Apparently applying a force or impulse doesn't activate the rigidbody
	void SetWorldPosition(const glm::vec3& pos);
	void ApplyCentralImpulse(const glm::vec3& impulse);
	void ApplyTorqueImpulse(const glm::vec3& torque);
};

#endif