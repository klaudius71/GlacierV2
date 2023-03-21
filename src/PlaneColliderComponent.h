#ifndef _PLANE_COLLIDER_COMPONENT
#define _PLANE_COLLIDER_COMPONENT

#include "GlacierCore.h"
#include "RigidbodyComponent.h"

struct GLACIER_API PlaneColliderComponent : public RigidbodyComponent
{
	PlaneColliderComponent(float friction, float rolling_friction, float restitution, float y_offset = 0.0f, const glm::vec3& normal = glm::vec3(0.0f, 1.0f, 0.0f));
	PlaneColliderComponent(PlaneColliderComponent&& o);
	PlaneColliderComponent& operator=(PlaneColliderComponent&& o);
	~PlaneColliderComponent() = default;
};

#endif