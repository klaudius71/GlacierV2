#include "gpch.h"
#include "LevelScript.h"
#include "RotatingScript.h"

void LevelScript::OnCreate()
{
}

void LevelScript::OnTick()
{
	static bool woah = true;
	
	if (Input::GetKeyDown(GLACIER_KEY::KEY_1) && woah)
	{
		woah = false;
		if (cube2.isExpired())
		{
			GameObjectRef cube1 = GetCurrentScene().FindGameObject("Cube1");
			cube2 = CreateGameObject("Cube3", cube1);
			//cube2->AddScript<RotatingScript>();
			cube2->GetComponent<TransformComponent>().position() = glm::vec3(0.0f, 10.0f, 0.0f);
			cube2->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), TextureLoader::Get("default"));
			cube2->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
		}
		cube2->RegisterToScene();
	}
	
	if (Input::GetKeyDown(GLACIER_KEY::KEY_2) && !woah)
	{
		woah = true;
		cube2->DeregisterFromScene();
	}

	if (Input::GetKeyDown(GLACIER_KEY::KEY_3) && !woah)
	{
		woah = true;
		GetCurrentScene().DestroyGameObject(cube2);
	}
}

void LevelScript::OnDestroy()
{
}
