#include "MainScene.h"
#include "LevelScript.h"
#include "RotatingScript.h"
#include "SpinningLightScript.h"
#include "PlayerTankPrefab.h"
#include "BSphereTestingScript.h"
#include "FPSCharacterControllerPrefab.h"

void MainScene::InitializeScene()
{
	GameObject level_script = CreateGameObject("Level Script");
	level_script->AddScript<LevelScript>();
	level_script->RegisterToScene();

	//GameObject tank = CreatePrefab<PlayerTankPrefab>();
	//tank->GetComponent<TransformComponent>().position().y = 10.5f;
	//tank->RegisterToScene();

	//GameObject camera = CreateGameObject("God Camera");
	//camera->EmplaceComponent<CameraComponent>().cam_pos = glm::vec3(0.0f, 100.0f, 50.0f);
	//camera->AddScript<CameraControllerScript>();
	//camera->RegisterToScene();

	//GameObject character = CreatePrefab<FPSCharacterControllerPrefab>();
	////character->GetComponent<CharacterControllerComponent>().m_controller->setWorldTransform(btTransform(btMatrix3x3::getIdentity(), btVector3(0.0f, 500.0f, 0.0f)));
	//character->RegisterToScene();

	GameObject dir_light = CreateGameObject("Directional Light");
	//const glm::vec3 light_dir = glm::normalize(glm::vec3(.967f, -1.0f, 0.254f));
	const glm::vec3 light_dir = glm::normalize(glm::vec3(-1, -1, -1));
	dir_light->EmplaceComponent<DirectionalLightComponent>(VertexTypes::PhongADS(glm::vec3(.45f), glm::vec3(.85f), glm::vec3(1.0f), 0), light_dir);
	//dir_light->AddScript<SpinningLightScript>();
	dir_light->RegisterToScene();

	//GameObject skybox = CreateGameObject("Skybox");
	//skybox->EmplaceComponent<SkyboxComponent>(TextureLoader::Get("Skybox"));
	//skybox->RegisterToScene();

	GameObject terrain = CreateGameObject("Terrain");
	//terrain->EmplaceComponent<MeshComponent>((const ModelOpenGL*)ModelLoader::Get("TempTerrain"), false);
	//terrain->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(.5f), 4.0f), TextureLoader::Get("Sand"));
	//terrain->EmplaceComponent<TriangleMeshColliderComponent>(ModelLoader::Get("TempTerrain"), 10.0f, 10.0f, 0.5f);
	terrain->RegisterToScene();

	RigidbodyComponent* rigidbody;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6 - i; j++)
		{
			GameObject cube = CreateGameObject("Cube1");
			cube->GetComponent<TransformComponent>().scale() = glm::vec3(3.0f);
			//cube->EmplaceComponent<MeshComponent>((const ModelOpenGL*)ModelLoader::Get("Box"));
			//cube->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Crate"))
			//	.norm_tex_id.x = TextureLoader::Get("CrateNormal");
	
			rigidbody = &cube->EmplaceComponent<BoxColliderComponent>(glm::vec3(15.0f, 15.0f, 15.0f), .1f, .25f, .5f);
			rigidbody->SetWorldPosition(glm::vec3(25.0f + float(j * 2 - (6 - i) + 1), 25.0f + float(i * 2 + 1), 0.0f) * 15.0f);
			cube->RegisterToScene();
		}
	}
	
	GameObject sphere = CreateGameObject("Sphere");
	//sphere->EmplaceComponent<MeshComponent>((const ModelOpenGL*)ModelLoader::Get("Sphere"));
	//sphere->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("Rock")).norm_tex_id.x = TextureLoader::Get("RockNormal");
	sphere->GetComponent<TransformComponent>().scale() = glm::vec3(20.0f);
	rigidbody = &sphere->EmplaceComponent<SphereColliderComponent>(20.0f);
	rigidbody->SetWorldPosition(glm::vec3(150.0f, 500.0f, 100.0f));
	sphere->AddScript<RotatingScript>();
	sphere->RegisterToScene();

	GameObject vampire = CreateGameObject("Vampire");
	//vampire->EmplaceComponent<SkeletalMeshComponent>((const ModelOpenGL*)ModelLoader::Get("Vampire"));
	vampire->GetComponent<TransformComponent>().position() = glm::vec3(-155.0f, 40.0f, 0.0f);
	//vampire->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 64.0f), TextureLoader::Get("VampireDiffuse")).norm_tex_id.x = TextureLoader::Get("VampireNormal");
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
