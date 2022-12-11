#include "gpch.h"
#include "RotatingScript.h"

RotatingScript::RotatingScript(const glm::vec3& rot_direction)
	: Script("RotatingScript"), rot_direction(rot_direction)
{
}

void RotatingScript::OnSceneEnter()
{
	transform = &GetGameObject()->GetComponent<TransformComponent>();
}
void RotatingScript::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	//transform->rotation() += rot_direction * dt;
	//glm::vec4& curr_pos = (*transform)[3];
	//curr_pos.y = sinf((float)glfwGetTime() * 2.0f) * 10.0f + 60.0f;

	static bool key_pressed = false;
	if (Input::GetKeyDown(GLACIER_KEY::KEY_F) && !key_pressed)
	{
		key_pressed = true;
		GetGameObject()->GetComponent<RigidbodyComponent>().rb->setLinearVelocity(btVector3(0.0f, 100.0f, -250.0f));
	}
	else if(!Input::GetKeyDown(GLACIER_KEY::KEY_F))
	{
		key_pressed = false;
	}

	const Font& font = FontLoader::Get("CascadiaMono20");
	const Framebuffer& fb = Renderer::GetMainFramebuffer();
	Renderer2D::PrintText(font, 0.0f, fb.GetSize().y - (font.GetGlyphWithMaxHeight().size.y * 0.5f) * 9.0f, Colors::Black, "Launch Ball: F");
}