#include "gpch.h"
#include "LevelScript.h"
#include "RotatingScript.h"

void LevelScript::OnCreate()
{
	//cube2 = SceneManager::GetCurrentScene()->CreateGameObject("Cube2");
	////cube2->AddScript<RotatingScript>();
	//cube2->GetComponent<TransformComponent>().position() = glm::vec3(0.0f, 1.0f, 0.0f);
	//cube2->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), TextureLoader::Get("default"));
	//cube2->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
}

void LevelScript::OnTick()
{
	//static bool woah = true;
	//
	//if (Input::GetKeyDown(GLACIER_KEY::KEY_1) && woah)
	//{
	//	woah = false;
	//	if (!cube2)
	//	{
	//		cube2 = SceneManager::GetCurrentScene()->CreateGameObject("Cube2");
	//		//cube2->AddScript<RotatingScript>();
	//		cube2->GetComponent<TransformComponent>().position() = glm::vec3(0.0f, 1.0f, 0.0f);
	//		cube2->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), TextureLoader::Get("default"));
	//		cube2->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
	//	}
	//	cube2->RegisterToScene();
	//}
	//
	//if (Input::GetKeyDown(GLACIER_KEY::KEY_2) && !woah)
	//{
	//	woah = true;
	//	cube2->DeregisterFromScene();
	//}

	//if (Input::GetKeyDown(GLACIER_KEY::KEY_3))
	//{
	//	woah = true;
	//	delete cube2;
	//	cube2 = nullptr;
	//}
}

void LevelScript::OnDestroy()
{
	//delete cube2;
}
