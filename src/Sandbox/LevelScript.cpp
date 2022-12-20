#include "gpch.h"
#include "LevelScript.h"
#include "RotatingScript.h"

LevelScript::LevelScript()
	: Script("LevelScript")
{
}

void LevelScript::OnSceneEnter()
{
}
void LevelScript::OnUpdate(float dt)
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
			cube2->GetComponent<TransformComponent>().position() = glm::vec3(-10.0f, 0.0f, 0.0f);
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

	const Framebuffer& fb = Renderer::GetMainFramebuffer();
	const Font& font = FontLoader::Get("CascadiaMono20");
	Renderer2D::PrintText(font, 0.0f, (float)fb.GetSize().y - font.GetGlyphWithMaxHeight().size.y * 0.5f, Colors::White, "update: %.4fms", Logger::GetFloat("update"));
	Renderer2D::PrintText(font, 200.0f, (float)fb.GetSize().y - font.GetGlyphWithMaxHeight().size.y * 0.5f, Colors::White, "physics: %.4fms", Logger::GetFloat("physics"));
	Renderer2D::PrintText(font, 420.0f, (float)fb.GetSize().y - font.GetGlyphWithMaxHeight().size.y * 0.5f, Colors::White, "draw: %.4fms", Logger::GetFloat("draw"));
	Renderer2D::PrintText(font, 600.0f, (float)fb.GetSize().y - font.GetGlyphWithMaxHeight().size.y * 0.5f, Colors::White, "3d: %.4fms", Logger::GetFloat("3d"));
	Renderer2D::PrintText(font, 750.0f, (float)fb.GetSize().y - font.GetGlyphWithMaxHeight().size.y * 0.5f, Colors::White, "2d: %.4fms", Logger::GetFloat("2d"));
	Renderer2D::PrintText(font, 0.0f, (float)fb.GetSize().y - (font.GetGlyphWithMaxHeight().size.y * 0.5f) * 3.0f, Colors::Blue, "frame_time: %.4fms", Logger::GetFloat("frame_time"));

}
void LevelScript::OnSceneExit()
{
}