#include "gpch.h"
#include "Physics.h"
#include "Scene.h"
#include "Components.h"
#include "Logger.h"

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
	entt::registry& registry = scn.GetRegistry();
	auto group = registry.group<RigidbodyComponent, TransformComponent>(entt::get<MeshComponent>);
	for (auto&& [entity, rigidbody, transform, mesh] : group.each())
	{
		rigidbody.motion_state->getWorldTransform(transf);
		glm::mat4 mat;
		transf.getOpenGLMatrix(glm::value_ptr(mat));
		transform.SetWorldMatrixKeepScale(mat);
	}
}

void Physics::addRigidbodyToWorld(btRigidBody* const rigidbody)
{
	dynamicsWorld->addRigidBody(rigidbody);
}
void Physics::removeRigidbodyFromWorld(btRigidBody* const rigidbody)
{
	dynamicsWorld->removeRigidBody(rigidbody);
}

void Physics::resetScene()
{
}
