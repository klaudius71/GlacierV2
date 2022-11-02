#include "gpch.h"
#include "LevelScript.h"
#include "RotatingScript.h"

LevelScript::LevelScript()
	: Script("LevelScript"), frame_time_index(0)
{
	frame_time_data.reserve(100);
	for (int i = 0; i < 100; i++)
		frame_time_data.emplace_back(0.0f);
}

void LevelScript::OnSceneEnter()
{
}
void LevelScript::OnUpdate()
{
	static bool woah = true;
	
	if (Input::GetKeyDown(GLACIER_KEY::KEY_1) && woah)
	{
		woah = false;
		if (cube2.isExpired())
		{
			GameObject cube1 = GetCurrentScene().FindGameObject("Cube1");
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

	const float avg_frame_time = std::accumulate(frame_time_data.cbegin(), frame_time_data.cend(), 0.0f) / 100.0f;
	const Framebuffer& fb = Renderer::GetMainFramebuffer();
	const Font& font = FontLoader::Get("CascadiaMono20");
	Renderer2D::PrintText(font, 0.0f, (float)fb.GetSize().y - font.GetGlyphWithMaxHeight().size.y * 0.5f, Colors::MediumBlue, "frame_time: %.3fms", avg_frame_time * 1000.0f);
}
void LevelScript::OnSceneExit()
{
}