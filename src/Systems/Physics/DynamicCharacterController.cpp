#include "gpch.h"
#include "DynamicCharacterController.h"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btDefaultMotionState.h"
#include "Physics.h"
#include "Renderer2D.h"
#include "FontLoader.h"

DynamicCharacterController::DynamicCharacterController(const float& height, const float& width, const float& stepHeight)
	: m_rayLambda{ 1.0f, 1.0f },
	m_halfHeight(height * 0.5f),
	m_walkSpeed(125.0f) // meters/sec
{
	UNREFERENCED_PARAMETER(stepHeight);
	btScalar sphereRadii[2]{ width, width };
	btVector3 spherePositions[2]{ btVector3(0.0, (height * 0.5f - width), 0.0), 
								  btVector3(0.0, (-height * 0.5f + width), 0.0) };

	m_shape = new btMultiSphereShape(&spherePositions[0], &sphereRadii[0], 2);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0, 100.0f, 0.0));
	m_motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(1000.0f, m_motionState, m_shape);
	m_rigidBody = new btRigidBody(cInfo);
	// kinematic vs. static doesn't work
	//m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	m_rigidBody->setSleepingThresholds(0.0f, 0.0f);
	m_rigidBody->setAngularFactor(0.0f);
	m_rigidBody->setFriction(.1f);
}
DynamicCharacterController::~DynamicCharacterController()
{
	delete m_rigidBody;
	delete m_motionState;
	delete m_shape;
}

void DynamicCharacterController::SetWalkDirection(const glm::vec3& direction)
{
	m_walkDirection.setValue(direction.x, direction.y, direction.z);
}

btMotionState* const DynamicCharacterController::GetMotionState()
{
	return m_motionState;
}

void DynamicCharacterController::AddToScene()
{
	Physics::AddAction(this);
	Physics::AddRigidbodyToWorld(m_rigidBody);
	m_rigidBody->setGravity(btVector3(0.0f, -198.1f, 0.0f));
}
void DynamicCharacterController::RemoveFromScene()
{
	Physics::RemoveRigidbodyFromWorld(m_rigidBody);
	Physics::RemoveAction(this);
}

void DynamicCharacterController::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
	preStep((btDiscreteDynamicsWorld*)collisionWorld);
	playerStep((btDiscreteDynamicsWorld*)collisionWorld, deltaTimeStep);
}

btCollisionObject* DynamicCharacterController::getCollisionObject()
{
	return m_rigidBody;
}

void DynamicCharacterController::preStep(const btDiscreteDynamicsWorld* collisionWorld)
{
	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform (xform);
	btVector3 down = -xform.getBasis()[1];
	btVector3 forward = xform.getBasis()[2];
	down.normalize ();
	forward.normalize();

	m_raySource[0] = xform.getOrigin();
	m_raySource[1] = xform.getOrigin();

	m_rayTarget[0] = m_raySource[0] + down * m_halfHeight * btScalar(1.1);
	m_rayTarget[1] = m_raySource[1] + forward * m_halfHeight * btScalar(1.1);

	class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
	{
	public:
		ClosestNotMe (btRigidBody* me) 
			: btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
		{
			m_me = me;
		}

		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
		{
			if (rayResult.m_collisionObject == m_me)
				return 1.0;

			return ClosestRayResultCallback::addSingleResult (rayResult, normalInWorldSpace
		);
	}
	protected:
		btRigidBody* m_me;
	};

	ClosestNotMe rayCallback(m_rigidBody);

	int i = 0;
	for (i = 0; i < 2; i++)
	{
		rayCallback.m_closestHitFraction = 1.0;
		collisionWorld->rayTest (m_raySource[i], m_rayTarget[i], rayCallback);
		if (rayCallback.hasHit())
		{
			m_rayLambda[i] = rayCallback.m_closestHitFraction;
		} else {
			m_rayLambda[i] = 1.0;
		}
	}
}
void DynamicCharacterController::playerStep(const btDiscreteDynamicsWorld* dynaWorld, btScalar dt)
{
	UNREFERENCED_PARAMETER(dynaWorld);
	UNREFERENCED_PARAMETER(dt);

	btVector3 linearVelocity = m_rigidBody->getLinearVelocity();
	
	//Renderer2D::PrintText(FontLoader::Get("CascadiaMono20"), 0.0f, 800.0f, Colors::Blue, "onGround()=%s", onGround() ? "true" : "false");

	if (m_walkDirection.fuzzyZero())
	{
		/* Dampen when on the ground and not being moved by the player */
		linearVelocity *= btVector3(.85f, 1.0f, .85f);
	}
	else
	{
		const btVector3 temp = m_walkDirection * m_walkSpeed;
		linearVelocity.setValue(temp.x(), linearVelocity.y(), temp.z());
	}

	//linearVelocity.setY(linearVelocity.y() + dynaWorld->getGravity().y() * dt);
	m_rigidBody->setLinearVelocity(linearVelocity);
}

bool DynamicCharacterController::canJump() const
{
	return onGround();
}

void DynamicCharacterController::jump()
{
	if (!canJump())
		return;

	btVector3 linearVelocity = m_rigidBody->getLinearVelocity();
	linearVelocity.setY(linearVelocity.y() + 20.0f);
	m_rigidBody->setLinearVelocity(linearVelocity);
	m_rayLambda[0] = 1.0f; // canJump() = false
	
	//btTransform xform;
	//m_rigidBody->getMotionState()->getWorldTransform(xform);
	//btVector3 up = xform.getBasis()[1];
	//up.normalize ();
	//btScalar magnitude = (btScalar(1.0)/m_rigidBody->getInvMass()) * btScalar(8.0);
	//m_rigidBody->applyCentralImpulse (up * magnitude);
}

bool DynamicCharacterController::onGround() const
{
	return m_rayLambda[0] < btScalar(1.0);
}