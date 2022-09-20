#include "gpch.h"
#include "Renderer2D.h"
#include "Glacier.h"
#include "Window.h"
#include "ModelLoader.h"

Renderer2D* Renderer2D::instance = nullptr;

Renderer2D::Renderer2D()
{
	const auto& window = Glacier::GetWindow();
	proj = glm::ortho(window.GetWindowWidth() * -0.5f, window.GetWindowWidth() * 0.5f, window.GetWindowHeight() * -0.5f, window.GetWindowHeight() * 0.5f);
	view = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Renderer2D::Initialize()
{
	instance = new Renderer2D;
}
void Renderer2D::Terminate()
{
	delete instance;
	instance = nullptr;
}

void Renderer2D::RenderComponents(entt::registry& registry)
{
	static const GLuint quad = ModelLoader::Get(PRELOADED_MODELS::QUAD)->GetVAO();

	auto group = registry.group<Render2DComponent>(entt::get<TransformComponent>);
	glBindVertexArray(quad);
	for (auto&& [entity, render, transform] : group.each())
	{
		glUseProgram(render.shad);
		
		glUniformMatrix4fv(glGetUniformLocation(render.shad, "proj_matrix"),  1, GL_FALSE, (const GLfloat*)&instance->proj);
		glUniformMatrix4fv(glGetUniformLocation(render.shad, "view_matrix"),  1, GL_FALSE, (const GLfloat*)&instance->view);
		glUniformMatrix4fv(glGetUniformLocation(render.shad, "world_matrix"), 1, GL_FALSE, (const GLfloat*)&transform);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render.tex_id);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}
