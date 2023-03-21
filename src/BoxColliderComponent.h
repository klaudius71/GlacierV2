#ifndef _BOX_COLLIDER_COMPONENT
#define _BOX_COLLIDER_COMPONENT

#include "GlacierCore.h"
#include "RigidbodyComponent.h"

struct GLACIER_API BoxColliderComponent : public RigidbodyComponent
{
	BoxColliderComponent(const glm::vec3& half_extents, float friction, float rolling_friction, float restitution, float mass = 100.0f);
	BoxColliderComponent(BoxColliderComponent&& o);
	BoxColliderComponent& operator=(BoxColliderComponent&& o);
	~BoxColliderComponent() = default;
};

#endif
