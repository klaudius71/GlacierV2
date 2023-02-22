#include "gpch.h"
#include "Physics.h"
#include "Scene.h"
#include "Components.h"
#include "Logger.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "DynamicCharacterController.h"

Physics& Physics::Instance()
{
    static Physics instance;
    return instance;
}

Physics::Physics()
	: collisionConfiguration(new btDefaultCollisionConfiguration),
	dispatcher(new btCollisionDispatcher(collisionConfiguration)),
	overlappingPairCache(new btDbvtBroadphase),
	solver(new btSequentialImpulseConstraintSolver),
	dynamicsWorld(new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration))
{
	dynamicsWorld->setGravity(btVector3(0.0f, -98.1f, 0.0f));
}
Physics::~Physics()
{
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
}

void Physics::simulatePhysics(const float& timestep, const int& max_substeps, const float& fixed_timestep, Scene& scn)
{
	GLACIER_LOG_FUNC_TIMER("physics");

	dynamicsWorld->stepSimulation(timestep, max_substeps, fixed_timestep);

	btTransform transf; 
	glm::mat4 mat;
	entt::registry& registry = scn.GetRegistry();
	auto group = registry.group<RigidbodyComponent, TransformComponent>();
	for (auto&& [entity, rigidbody, transform] : group.each())
	{
		rigidbody.motion_state->getWorldTransform(transf);
		transf.getOpenGLMatrix(glm::value_ptr(mat));
		transform.SetWorldMatrixKeepScale(mat);
	}

	auto view = registry.view<CharacterControllerComponent>();
	for (auto&& [entity, character_controller] : view.each())
	{
		character_controller.m_controller->GetMotionState()->getWorldTransform(transf);
		transf.getOpenGLMatrix(glm::value_ptr(mat));
		registry.get<TransformComponent>(entity).SetWorldMatrixKeepScale(mat);
	}
}
void Physics::addCollisionObject(btCollisionObject* const obj, const int& collisionFilterGroup, const int& collisionFilterMask)
{
	dynamicsWorld->addCollisionObject(obj, collisionFilterGroup, collisionFilterMask);
}
void Physics::addAction(btActionInterface* const action)
{
	dynamicsWorld->addAction(action);
}
void Physics::addRigidbodyToWorld(btRigidBody* const rigidbody)
{
	dynamicsWorld->addRigidBody(rigidbody);
}
void Physics::removeCollisionObject(btCollisionObject* const obj)
{
	dynamicsWorld->removeCollisionObject(obj);
}
void Physics::removeAction(btActionInterface* const action)
{
	dynamicsWorld->removeAction(action);
}
void Physics::removeRigidbodyFromWorld(btRigidBody* const rigidbody)
{
	dynamicsWorld->removeRigidBody(rigidbody);
}