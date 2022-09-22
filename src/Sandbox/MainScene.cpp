#include "gpch.h"
#include "MainScene.h"
#include "LevelScript.h"
#include "RotatingScript.h"
#include "SpinningLightScript.h"

void MainScene::InitializeScene()
{
	//level_script = new GameObject("Level Script");
	//level_script->AddScript<LevelScript>();
	//level_script->RegisterToScene();
	
	auto& camera = CreateGameObject("Camera");
	camera.EmplaceComponent<CameraComponent>(glm::perspective(glm::radians(70.0f), (float)Glacier::GetWindow().GetWindowWidth() / (float)Glacier::GetWindow().GetWindowHeight(), 0.1f, 10000.0f),
											  glm::lookAt(glm::vec3(0.0f, 100.0f, 20.0f), glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	camera.AddScript<CameraControllerScript>();
	camera.RegisterToScene();

	auto& dir_light = CreateGameObject("Directional Light");
	dir_light.EmplaceComponent<DirectionalLightComponent>(glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
	//dir_light->AddScript<SpinningLightScript>();
	dir_light.RegisterToScene();

	auto& terrain = CreateGameObject("Terrain");
	terrain.EmplaceComponent<MeshComponent>(ModelLoader::Get("TempTerrain"));
	terrain.EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(.1f), 1.0f), TextureLoader::Get("Sand"));
	terrain.RegisterToScene();

	auto& cube = CreateGameObject("Cube1");
	cube.GetComponent<TransformComponent>().position().y = 60.0f;
	cube.EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"));
	cube.EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	cube.RegisterToScene();
	cube.AddScript<RotatingScript>();

	auto& cube2 = CreateGameObject("Cube2", cube);
	cube2.GetComponent<TransformComponent>().position().x = 20.0f;
	cube2.EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"));
	cube2.EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	cube2.RegisterToScene();
}

void MainScene::EndScene()
{
}
