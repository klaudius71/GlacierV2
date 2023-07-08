#ifndef _TRIANGLE_MESH_COLLIDER_COMPONENT
#define _TRIANGLE_MESH_COLLIDER_COMPONENT

#include "GlacierCore.h"
#include "RigidbodyComponent.h"

class btStridingMeshInterface;
class Model;

struct GLACIER_API TriangleMeshColliderComponent : public RigidbodyComponent
{
	btStridingMeshInterface* mesh;

	TriangleMeshColliderComponent(const Model* model, float friction, float rolling_friction, float restitution);
	TriangleMeshColliderComponent(TriangleMeshColliderComponent&& o);
	TriangleMeshColliderComponent& operator=(TriangleMeshColliderComponent&& o);
	~TriangleMeshColliderComponent();
};

#endif