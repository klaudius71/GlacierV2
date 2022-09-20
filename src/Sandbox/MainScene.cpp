#include "gpch.h"
#include "MainScene.h"
#include "LevelScript.h"
#include "RotatingScript.h"
#include "SpinningLightScript.h"

void MainScene::InitializeScene()
{
	level_script = new GameObject("Level Script");
	level_script->AddScript<LevelScript>();
	level_script->RegisterToScene();
	
	camera = new GameObject("Camera");
	camera->EmplaceComponent<CameraComponent>(glm::perspective(glm::radians(70.0f), (float)Glacier::GetWindow().GetWindowWidth() / (float)Glacier::GetWindow().GetWindowHeight(), 0.1f, 10000.0f),
											  glm::lookAt(glm::vec3(0.0f, 100.0f, 20.0f), glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	camera->AddScript<CameraControllerScript>();
	camera->RegisterToScene();

	dir_light = new GameObject("Directional Light");
	dir_light->EmplaceComponent<DirectionalLightComponent>(glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
	//dir_light->AddScript<SpinningLightScript>();
	dir_light->RegisterToScene();

	terrain = new GameObject("Terrain");
	terrain->EmplaceComponent<MeshComponent>(ModelLoader::Get("TempTerrain"));
	terrain->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(.1f), 1.0f), TextureLoader::Get("Sand"));
	terrain->RegisterToScene();

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			GameObject*& cube = cubes.emplace_back(new GameObject("Cube" + (i * 2 + j)));
			cube->GetComponent<TransformComponent>().position() = glm::vec3(-20.0f + (i * 40.0f), 60.0f, -20.0f + (j * 40.0f));
			cube->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
			cube->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 8.0f), TextureLoader::Get("Crate"));
			cube->AddScript<RotatingScript>();
			cube->RegisterToScene();
		}
	}
}

void MainScene::EndScene()
{
	delete level_script;
	delete camera;
	delete dir_light;
	delete terrain;
	for (auto& x : cubes)
		delete x;
}
