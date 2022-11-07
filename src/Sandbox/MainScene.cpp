#include "gpch.h"
#include "MainScene.h"
#include "LevelScript.h"
#include "RotatingScript.h"
#include "SpinningLightScript.h"
#include "PlayerTankPrefab.h"

void MainScene::InitializeScene()
{
	GameObject level_script = CreateGameObject("Level Script");
	level_script->AddScript<LevelScript>();
	level_script->RegisterToScene();

	//GameObject camera = CreateGameObject("God Camera");
	//camera->EmplaceComponent<CameraComponent>().cam_pos = glm::vec3(0.0f, 50.0f, 50.0f);
	//camera->AddScript<CameraControllerScript>();
	//camera->RegisterToScene();

	GameObject dir_light = CreateGameObject("Directional Light");
	//const glm::vec3 light_dir = glm::normalize(glm::vec3(.967f, -1.0f, 0.254f));
	const glm::vec3 light_dir = glm::normalize(glm::vec3(-1, -1, -1));
	dir_light->EmplaceComponent<DirectionalLightComponent>(VertexTypes::PhongADS(glm::vec3(.45f), glm::vec3(.85f), glm::vec3(1.0f), 0), light_dir);
	//dir_light->AddScript<SpinningLightScript>();
	dir_light->RegisterToScene();

	GameObject skybox = CreateGameObject("Skybox");
	skybox->EmplaceComponent<SkyboxComponent>(TextureLoader::Get("Skybox"));
	skybox->RegisterToScene();

	GameObject terrain = CreateGameObject("Terrain");
	terrain->EmplaceComponent<MeshComponent>(ModelLoader::Get("TempTerrain1"), false);
	terrain->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(.1f), 1.0f), TextureLoader::Get("Sand"));
	terrain->RegisterToScene();

	GameObject cube = CreateGameObject("Cube1");
	cube->GetComponent<TransformComponent>().position().y = 15.0f;
	//cube->GetComponent<TransformComponent>().rotation().z = 3.141529f * (1.0f/8.0f);
	cube->GetComponent<TransformComponent>().scale() = glm::vec3(3.0f);
	cube->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"))
		.norm_tex_id.x = TextureLoader::GetConst("CrateNormal");
	cube->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	//cube->AddScript<RotatingScript>();
	cube->RegisterToScene();
	
	//GameObject cube2 = CreateGameObject("Cube2");
	//cube2->GetComponent<TransformComponent>().position() = glm::vec3(40.0f, 90.0f, 40.0f);
	//cube2->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"));
	//cube2->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	//cube2->RegisterToScene();

	GameObject sphere = CreateGameObject("Sphere");
	sphere->EmplaceComponent<MeshComponent>(ModelLoader::Get("Sphere"));
	sphere->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Rock")).norm_tex_id.x = TextureLoader::Get("RockNormal");
	sphere->GetComponent<TransformComponent>().scale() = glm::vec3(20.0f);
	sphere->GetComponent<TransformComponent>().position() = glm::vec3(50.0f, 20.0f, 0.0f);
	sphere->AddScript<RotatingScript>();
	sphere->RegisterToScene();

	//GameObject sphere2 = CreateGameObject("Sphere2");
	//sphere2->EmplaceComponent<MeshComponent>(ModelLoader::Get("Sphere2"));
	//sphere2->GetComponent<TransformComponent>().scale() = glm::vec3(20.0f);
	//sphere2->GetComponent<TransformComponent>().position() = glm::vec3(100.0f, 20.0f, 0.0f);
	//sphere2->RegisterToScene();

	GameObject tank = CreatePrefab<PlayerTankPrefab>();
	tank->GetComponent<TransformComponent>().position().y = 10.5f;
	tank->RegisterToScene();
}

void MainScene::EndScene()
{
}
