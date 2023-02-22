// Modified from CharacterDemo
#ifndef _DYNAMIC_CHARACTER_CONTROLLER
#define _DYNAMIC_CHARACTER_CONTROLLER

#include "LinearMath/btVector3.h"
#include "BulletDynamics/Character/btCharacterControllerInterface.h"

class btCollisionShape;
class btRigidBody;
class btCollisionWorld;

class DynamicCharacterController : public btActionInterface
{
protected:
	btScalar m_halfHeight;
	btCollisionShape* m_shape;
	btDefaultMotionState* m_motionState;
	btRigidBody* m_rigidBody;

	btVector3 m_raySource[2];
	btVector3 m_rayTarget[2];
	btScalar m_rayLambda[2];
	btVector3 m_rayNormal[2];
	
	btScalar m_walkSpeed;

	btVector3 m_walkDirection;

	virtual void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);
	virtual void debugDraw(btIDebugDraw* debugDrawer) { UNREFERENCED_PARAMETER(debugDrawer); }

public:
	DynamicCharacterController(const float& height = 2.0f, const float& width = 0.25f, const float& stepHeight = 0.25f);
	~DynamicCharacterController();

	void SetWalkDirection(const glm::vec3& direction);

	btMotionState* const GetMotionState();

	void AddToScene();
	void RemoveFromScene();

	//void reset();
	//void warp(const btVector3& origin);
	//void registerPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher);

	btCollisionObject* getCollisionObject();

	void preStep(const btDiscreteDynamicsWorld* collisionWorld);
	void playerStep(const btDiscreteDynamicsWorld* collisionWorld,btScalar dt);
	bool canJump() const;
	void jump();

	bool onGround() const;
};

#endif
