#include "gpch.h"
#include "MainScene.h"
#include "LevelScript.h"
#include "RotatingScript.h"
#include "SpinningLightScript.h"

void MainScene::InitializeScene()
{
	GameObjectRef level_script = CreateGameObject("Level Script");
	level_script->AddScript<LevelScript>();
	level_script->RegisterToScene();
	
	GameObjectRef camera = CreateGameObject("Camera");
	camera->EmplaceComponent<CameraComponent>(glm::perspective(glm::radians(70.0f), (float)Glacier::GetWindow().GetWindowWidth() / (float)Glacier::GetWindow().GetWindowHeight(), 0.1f, 10000.0f),
											  glm::vec3(0.0f, 100.0f, 20.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	camera->AddScript<CameraControllerScript>();
	camera->RegisterToScene();

	//GameObjectRef dir_light = CreateGameObject("Directional Light");
	//dir_light->EmplaceComponent<DirectionalLightComponent>(VertexTypes::PhongADS(glm::vec3(0.45f), glm::vec3(.8f), glm::vec3(1.0f), 0), glm::normalize(-glm::vec3(-0.43f, 0.896f, -0.113f)));
	////dir_light->AddScript<SpinningLightScript>();
	//dir_light->RegisterToScene();

	GameObjectRef skybox = CreateGameObject("Skybox");
	skybox->EmplaceComponent<SkyboxComponent>(TextureLoader::Get("Skybox"));
	skybox->RegisterToScene();

	GameObjectRef terrain = CreateGameObject("Terrain");
	terrain->EmplaceComponent<MeshComponent>(ModelLoader::Get("TempTerrain"), false);
	terrain->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(.1f), 1.0f), TextureLoader::Get("Sand"));
	terrain->RegisterToScene();

	GameObjectRef cube = CreateGameObject("Cube1");
	cube->GetComponent<TransformComponent>().position().y = 70.0f;
	//cube->GetComponent<TransformComponent>().rotation().z = 3.141529f * (1.0f/8.0f);
	cube->GetComponent<TransformComponent>().scale() = glm::vec3(3.0f);
	cube->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"));
	cube->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	cube->AddScript<RotatingScript>();
	cube->RegisterToScene();

	GameObjectRef cube2 = CreateGameObject("Cube2");
	cube2->GetComponent<TransformComponent>().position() = glm::vec3(40.0f, 90.0f, 40.0f);
	cube2->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"));
	cube2->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	cube2->RegisterToScene();
}

void MainScene::EndScene()
{
}
