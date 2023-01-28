#include "gpch.h"
#include "DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent(const VertexTypes::DirectionalLight& dir_light)
	: light(dir_light)
{}
DirectionalLightComponent::DirectionalLightComponent(const VertexTypes::PhongADS& ads, const glm::vec3& dir)
	: light(ads, dir)
{}
DirectionalLightComponent::DirectionalLightComponent(const glm::vec3& dir)
	: light(VertexTypes::PhongADS(glm::vec3(.3f), glm::vec3(1.0f), glm::vec3(1.0f), 0), dir)
{}