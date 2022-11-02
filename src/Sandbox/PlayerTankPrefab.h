#include "GlacierAPI.h"
#include "TankControllerScript.h"

struct PlayerTankPrefab : public Prefab
{
	virtual GameObject ApplyPrefab(Scene& scn) override
	{
		GameObject base = scn.CreateGameObject("Tank Body");
		base->EmplaceComponent<MeshComponent>(ModelLoader::Get("TankBase"));
		base->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), TextureLoader::Get("Desert"));
		CameraComponent& camera_component = base->EmplaceComponent<CameraComponent>(glm::mat4(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(45.0f));
		CameraComponent::ResetProjectionToSize(camera_component, Renderer::GetMainFramebuffer().GetSize().x, Renderer::GetMainFramebuffer().GetSize().y);
		base->AddScript<TankControllerScript>();
		base->GetComponent<TransformComponent>().scale() = glm::vec3(10.0f);
		
		GameObject wheels = scn.CreateGameObject("Tank", base);
		wheels->EmplaceComponent<MeshComponent>(ModelLoader::Get("TankWheels"));
		wheels->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), TextureLoader::Get("Desert"));
		wheels->GetComponent<TransformComponent>().position() = glm::vec3(0.0f, -0.52f, 1.25f);
		wheels->RegisterToScene();

		GameObject turret = scn.CreateGameObject("Tank Turret", base);
		turret->EmplaceComponent<MeshComponent>(ModelLoader::Get("TankTurret"));
		turret->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), TextureLoader::Get("Desert"));
		turret->GetComponent<TransformComponent>().position() = glm::vec3(0.0f, 1.0f, 2.2f);
		turret->RegisterToScene();

		GameObject turret_barrel = scn.CreateGameObject("Tank Turret Barrel", turret);
		turret_barrel->EmplaceComponent<MeshComponent>(ModelLoader::Get("TankTurretBarrel"));
		turret_barrel->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), TextureLoader::Get("Desert"));
		turret_barrel->GetComponent<TransformComponent>().position() = glm::vec3(0.0f, -0.3f, 0.955f);
		turret_barrel->RegisterToScene();

		return base;
	}
};