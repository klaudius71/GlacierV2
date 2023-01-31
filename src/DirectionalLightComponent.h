#ifndef _DIRECTIONAL_LIGHT_COMPONENT
#define _DIRECTIONAL_LIGHT_COMPONENT

#include "VertexTypes.h"
#include "GlacierCore.h"

struct GLACIER_API DirectionalLightComponent
{
	VertexTypes::DirectionalLight light;

	DirectionalLightComponent() = default;
	DirectionalLightComponent(const VertexTypes::DirectionalLight& dir_light);
	DirectionalLightComponent(const VertexTypes::PhongADS& ads, const glm::vec3& dir);
	DirectionalLightComponent(const glm::vec3& dir);
	DirectionalLightComponent(DirectionalLightComponent&& o) = default;
	DirectionalLightComponent& operator=(DirectionalLightComponent&& o) = default;
	~DirectionalLightComponent() = default;
};

#endif
