#ifndef _PHYSICS
#define _PHYSICS

class Scene;

enum PHYSICS_PLANE
{
	PLANE_SHAPE
};
enum PHYSICS_BOX
{
	BOX_SHAPE
};
enum PHYSICS_SPHERE
{
	SPHERE_SHAPE
};

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
	void addRigidbodyToWorld(btRigidBody* const rigidbody);
	void removeRigidbodyFromWorld(btRigidBody* const rigidbody);
	void resetScene();

public:
	static void Initialize() { Instance(); }
	static void SimulatePhysics(const float& timestep, const int& max_substeps, const float& fixed_timestep, Scene& scn) { Instance().simulatePhysics(timestep, max_substeps, fixed_timestep, scn); }
	static void AddRigidbodyToWorld(btRigidBody* const rigidbody) { Instance().addRigidbodyToWorld(rigidbody); }
	static void RemoveRigidbodyFromWorld(btRigidBody* const rigidbody) { Instance().removeRigidbodyFromWorld(rigidbody); }
	static void ResetScene() { Instance().resetScene(); }
};

#endif