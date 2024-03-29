#include "LevelScript.h"
#include "RotatingScript.h"

LevelScript::LevelScript()
	: Script("LevelScript"), frames(0), update(0.0f), physics(0.0f), draw(0.0f), three(0.0f), two(0.0f), animation(0.0f), frame_time(0.0f)
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
			//cube2->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), TextureLoader::Get("default"));
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

	if ((float)frames > (0.1f * (1.0f / dt)))
	{
		frames = 0;
		update = Logger::GetFloat("update");
		physics = Logger::GetFloat("physics");
		draw = Logger::GetFloat("draw");
		three = Logger::GetFloat("3d");
		two = Logger::GetFloat("2d");
		animation = Logger::GetFloat("animation");
		frame_time = dt * 1000.0f;

		static char buf[128];
		sprintf_s(buf, "Glacier V2  FPS: %.1f", 1.0f / dt);
		Glacier::GetWindow().SetWindowTitle(buf);
	}
	frames++;

	const Window& window = Glacier::GetWindow();
	const Font& font = FontLoader::Get("CascadiaMono20");
	const float screen_y = (float)window.GetWindowHeight() - font.GetGlyphWithMaxHeight().size.y;
	Renderer2D::PrintText(font, 0.0f, screen_y, Colors::White, "update: %.4fms physics: %.4fms draw: %.4fms 3d: %.4fms 2d: %.4fms animation: %.4fms", update, physics, draw, three, two, animation);
	Renderer2D::PrintText(font, 0.0f, (float)window.GetWindowHeight() - (font.GetGlyphWithMaxHeight().size.y) * 2.0f, Colors::Blue, "frame_time: %.4fms", frame_time);
}
void LevelScript::OnSceneExit()
{
}