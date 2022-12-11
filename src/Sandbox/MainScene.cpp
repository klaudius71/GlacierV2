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

	//GameObject tank = CreatePrefab<PlayerTankPrefab>();
	//tank->GetComponent<TransformComponent>().position().y = 10.5f;
	//tank->RegisterToScene();

	GameObject camera = CreateGameObject("God Camera");
	camera->EmplaceComponent<CameraComponent>().cam_pos = glm::vec3(0.0f, 50.0f, 50.0f);
	camera->AddScript<CameraControllerScript>();
	camera->RegisterToScene();

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
	terrain->EmplaceComponent<RigidbodyComponent>(PLANE_SHAPE);
	terrain->RegisterToScene();

	RigidbodyComponent* rigidbody;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6 - i; j++)
		{
			GameObject cube = CreateGameObject("Cube1");
			cube->GetComponent<TransformComponent>().scale() = glm::vec3(3.0f);
			cube->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
			cube->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"))
				.norm_tex_id.x = TextureLoader::GetConst("CrateNormal");

			rigidbody = &cube->EmplaceComponent<RigidbodyComponent>(BOX_SHAPE, 15.0f, 15.0f, 15.0f);
			rigidbody->rb->setWorldTransform(btTransform(btMatrix3x3::getIdentity(), btVector3(float(j * 2 - (6 - i) + 1), float(i * 2 + 1), 0.0f) * 15.0f));
			cube->RegisterToScene();
		}
	}
	
	GameObject sphere = CreateGameObject("Sphere");
	sphere->EmplaceComponent<MeshComponent>(ModelLoader::Get("Sphere"));
	sphere->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Rock")).norm_tex_id.x = TextureLoader::Get("RockNormal");
	sphere->GetComponent<TransformComponent>().scale() = glm::vec3(20.0f);
	//sphere->GetComponent<TransformComponent>().position() = glm::vec3(18.0f, 68.0f, 86.0f);
	rigidbody = &sphere->EmplaceComponent<RigidbodyComponent>(SPHERE_SHAPE, 20.0f);
	rigidbody->rb->setWorldTransform(btTransform(btQuaternion(0.0f, 0.0f, 0.0f), btVector3(0.0f, 20.0f, 200.0f)));
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

	//GameObject bsphere = CreateGameObject("BSphere");
	//bsphere->EmplaceComponent<MeshComponent>(ModelLoader::Get("Sphere"), false);
	//bsphere->AddScript<BSphereTestingScript>(vampire);
	//bsphere->RegisterToScene();
}

void MainScene::EndScene()
{
}
