#ifndef _SPHERE_COLLIDER_COMPONENT
#define _SPHERE_COLLIDER_COMPONENT

#include "GlacierCore.h"
#include "RigidbodyComponent.h"

struct GLACIER_API SphereColliderComponent : public RigidbodyComponent
{
	SphereColliderComponent(float radius, float mass = 100.0f, float rolling_friction = 0.9f, float friction = 1.0f, float restitution = 0.5f);
	SphereColliderComponent(SphereColliderComponent&& o);
	SphereColliderComponent& operator=(SphereColliderComponent&& o);
	~SphereColliderComponent() = default;
};

#endif
