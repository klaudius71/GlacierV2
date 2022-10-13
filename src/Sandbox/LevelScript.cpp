#include "gpch.h"
#include "LevelScript.h"
#include "RotatingScript.h"

void LevelScript::OnCreate()
{
	frame_time_data.reserve(100);
	for (int i = 0; i < 100; i++)
		frame_time_data.emplace_back(0.0f);

	frame_time_index = 0;
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
			//cube2->AddScript<RotatingScript>(glm::vec3(1.0f, 0.0f, 0.0f));
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

	frame_time_data[frame_time_index++] = TimeManager::GetDeltaTime();
	if (frame_time_index == 100)
		frame_time_index = 0;

	float avg_frame_time = 0.0f;
	for (const float& frame_time : frame_time_data)
		avg_frame_time += frame_time;

	avg_frame_time /= 100.0f;
	const Window& window = Glacier::GetWindow();
	Renderer2D::PrintText(FontLoader::Get("Times40"), 10.0f, window.GetWindowHeight() - 29.0f, Colors::Black, "FPS: %d", (int)(std::roundf(1.0f / avg_frame_time)));
}

void LevelScript::OnDestroy()
{
}
