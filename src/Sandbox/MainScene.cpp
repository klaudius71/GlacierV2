#include "gpch.h"
#include "MainScene.h"
#include "LevelScript.h"
#include "RotatingScript.h"
#include "SpinningLightScript.h"

void MainScene::InitializeScene()
{
	GameObject level_script = CreateGameObject("Level Script");
	level_script->AddScript<LevelScript>();
	level_script->RegisterToScene();
	
	GameObject camera = CreateGameObject("Camera");
	CameraComponent& camera_component = camera->EmplaceComponent<CameraComponent>(glm::mat4(1.0f), glm::vec3(0.0f, 100.0f, 20.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	CameraComponent::ResetProjectionToSize(camera_component, Glacier::GetWindow().GetWindowWidth(), Glacier::GetWindow().GetWindowHeight());
	camera->AddScript<CameraControllerScript>();
	camera->RegisterToScene();

	GameObject crosshair = CreateGameObject("Crosshair", camera);
	crosshair->EmplaceComponent<SpriteComponent>(TextureLoader::Get("Crosshair"), Renderer2D::SCREEN_ANCHOR::MIDDLE_CENTER);
	crosshair->GetComponent<TransformComponent>().scale() = glm::vec3(2.0f, 2.0f, 1.0f);
	//crosshair->GetComponent<TransformComponent>().position() = glm::vec3(glm::vec2(Renderer::GetMainFramebuffer().GetSize().x, Renderer::GetMainFramebuffer().GetSize().y) * 0.5f, 0.0f);
	crosshair->RegisterToScene();

	GameObject dir_light = CreateGameObject("Directional Light");
	dir_light->EmplaceComponent<DirectionalLightComponent>(VertexTypes::PhongADS(glm::vec3(.45f), glm::vec3(.55f), glm::vec3(1.0f), 0), glm::normalize(-glm::vec3(-0.43f, 0.896f, -0.113f)));
	//dir_light->AddScript<SpinningLightScript>();
	dir_light->RegisterToScene();

	GameObject skybox = CreateGameObject("Skybox");
	skybox->EmplaceComponent<SkyboxComponent>(TextureLoader::Get("Skybox"));
	skybox->RegisterToScene();

	GameObject terrain = CreateGameObject("Terrain");
	terrain->EmplaceComponent<MeshComponent>(ModelLoader::Get("TempTerrain"), false);
	terrain->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(.5f), 1.0f), TextureLoader::Get("Sand"));
	terrain->RegisterToScene();

	GameObject cube = CreateGameObject("Cube1");
	cube->GetComponent<TransformComponent>().position().y = 70.0f;
	//cube->GetComponent<TransformComponent>().rotation().z = 3.141529f * (1.0f/8.0f);
	cube->GetComponent<TransformComponent>().scale() = glm::vec3(3.0f);
	cube->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"));
	cube->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	cube->AddScript<RotatingScript>();
	cube->RegisterToScene();

	GameObject cube2 = CreateGameObject("Cube2");
	cube2->GetComponent<TransformComponent>().position() = glm::vec3(40.0f, 90.0f, 40.0f);
	cube2->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"));
	cube2->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	cube2->RegisterToScene();
}

void MainScene::EndScene()
{
}
