#include "gpch.h"
#include "MainScene.h"
#include "LevelScript.h"
#include "RotatingScript.h"
#include "SpinningLightScript.h"
#include "PlayerTankPrefab.h"
#include "BSphereTestingScript.h"

void MainScene::InitializeScene()
{
	GameObject level_script = CreateGameObject("Level Script");
	level_script->AddScript<LevelScript>();
	level_script->RegisterToScene();

	GameObject tank = CreatePrefab<PlayerTankPrefab>();
	tank->GetComponent<TransformComponent>().position().y = 10.5f;
	tank->RegisterToScene();

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
	cube->GetComponent<TransformComponent>().position() = glm::vec3(-41.0f, 50.0f, 30.0f);
	cube->GetComponent<TransformComponent>().rotation() = glm::radians(glm::vec3(-5.0f, 0.0f, 7.0f));
	cube->GetComponent<TransformComponent>().scale() = glm::vec3(3.0f);
	cube->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"))
		.norm_tex_id.x = TextureLoader::GetConst("CrateNormal");
	cube->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	//cube->AddScript<RotatingScript>();
	cube->RegisterToScene();
	
	GameObject sphere = CreateGameObject("Sphere");
	sphere->EmplaceComponent<MeshComponent>(ModelLoader::Get("Sphere"));
	sphere->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Rock")).norm_tex_id.x = TextureLoader::Get("RockNormal");
	sphere->GetComponent<TransformComponent>().scale() = glm::vec3(20.0f);
	sphere->GetComponent<TransformComponent>().position() = glm::vec3(18.0f, 68.0f, 86.0f);
	sphere->AddScript<RotatingScript>();
	sphere->RegisterToScene();

	GameObject vampire = CreateGameObject("Vampire");
	vampire->EmplaceComponent<SkeletalMeshComponent>(ModelLoader::Get("Vampire"));
	vampire->GetComponent<TransformComponent>().position() = glm::vec3(-155.0f, 0.0f, 0.0f);
	vampire->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("VampireDiffuse")).norm_tex_id.x = TextureLoader::Get("VampireNormal");
	vampire->EmplaceComponent<SkeletalAnimationComponent>(SkeletalAnimationLoader::Get("VampireIdle"));
	vampire->RegisterToScene();

	//GameObject dude = CreateGameObject("Dude");
	//dude->EmplaceComponent<SkeletalMeshComponent>(ModelLoader::Get("Dude"));
	//dude->GetComponent<TransformComponent>().position() = glm::vec3(-155.0f, 0.0f, 0.0f);
	//dude->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f), 128.0f), TextureLoader::Get("Dude")).norm_tex_id.x = TextureLoader::Get("DudeNormal");
	//dude->EmplaceComponent<SkeletalAnimationComponent>(SkeletalAnimationLoader::Get("DudeIdle"));
	//dude->RegisterToScene();

	GameObject tankbase = CreateGameObject("TankBase");
	tankbase->EmplaceComponent<MeshComponent>(ModelLoader::Get("TankBase"));
	tankbase->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), TextureLoader::Get("Desert"));
	tankbase->GetComponent<TransformComponent>().scale() = glm::vec3(10.0f);
	tankbase->RegisterToScene();

	//GameObject bsphere = CreateGameObject("BSphere");
	//bsphere->EmplaceComponent<MeshComponent>(ModelLoader::Get("Sphere"), false);
	//bsphere->AddScript<BSphereTestingScript>(vampire);
	//bsphere->RegisterToScene();
}

void MainScene::EndScene()
{
}
