#ifndef _PHYSICS
#define _PHYSICS

class Scene;

class Physics
{
private:
	static Physics& Instance();
	Physics();
	Physics(const Physics&) = delete;
	Physics& operator=(const Physics&) = delete;
	~Physics();

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	void simulatePhysics(const float& timestep, const int& max_substeps, const float& fixed_timestep, Scene& scn);
	void addCollisionObject(btCollisionObject* const obj, const int& collisionFilterGroup, const int& collisionFilterMask);
	void addAction(btActionInterface* const action);
	void addRigidbodyToWorld(btRigidBody* const rigidbody);
	void removeCollisionObject(btCollisionObject* const obj);
	void removeAction(btActionInterface* const action);
	void removeRigidbodyFromWorld(btRigidBody* const rigidbody);
	//void resetScene();

public:
	static void Initialize() { Instance(); }
	static void SimulatePhysics(const float& timestep, const int& max_substeps, const float& fixed_timestep, Scene& scn) { Instance().simulatePhysics(timestep, max_substeps, fixed_timestep, scn); }
	static void AddCollisionObject(btCollisionObject* const obj, const int& collisionFilterGroup = btBroadphaseProxy::StaticFilter, const int& collisionFilterMask = btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter) { Instance().addCollisionObject(obj, collisionFilterGroup, collisionFilterMask); }
	static void AddAction(btActionInterface* const action) { Instance().addAction(action); }
	static void AddRigidbodyToWorld(btRigidBody* const rigidbody) { Instance().addRigidbodyToWorld(rigidbody); }
	static void RemoveCollisionObject(btCollisionObject* const obj) { Instance().removeCollisionObject(obj); }
	static void RemoveAction(btActionInterface* const action) { Instance().removeAction(action); }
	static void RemoveRigidbodyFromWorld(btRigidBody* const rigidbody) { Instance().removeRigidbodyFromWorld(rigidbody); }
	//static void ResetScene() { Instance().resetScene(); }
};

#endif