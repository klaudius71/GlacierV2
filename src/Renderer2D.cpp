#include "gpch.h"
#include "Renderer2D.h"
#include "Glacier.h"
#include "Window.h"
#include "Scene.h"
#include "ModelLoader.h"
#include "Font.h"
#include "ShaderLoader.h"
#include "Shader.h"

Renderer2D* Renderer2D::instance = nullptr;

Renderer2D::Renderer2D()
{
	const auto& window = Glacier::GetWindow();
	proj = glm::ortho(window.GetWindowWidth() * -0.5f, window.GetWindowWidth() * 0.5f, window.GetWindowHeight() * -0.5f, window.GetWindowHeight() * 0.5f);
	view = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	debug_text_queue.reserve(RESERVED_DEBUG_TEXT_QUERIES);
}

void Renderer2D::Initialize()
{
	instance = new Renderer2D;
}
void Renderer2D::UpdateScreenSize(const int& width, const int& height)
{
	instance->proj = glm::ortho(width * -0.5f, width * 0.5f, height * -0.5f, height * 0.5f);
}
void Renderer2D::Terminate()
{
	delete instance;
	instance = nullptr;
}

void Renderer2D::RenderComponents(Scene& scn)
{
	static const GLuint quad = ModelLoader::Get(PRELOADED_MODELS::QUAD)->GetVAO();

	auto group = scn.GetRegistry().group<Render2DComponent>(entt::get<TransformComponent>);
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


	for (const auto& entry : instance->debug_text_queue)
		RenderText(entry.font, entry.pos.x, entry.pos.y, entry.text);

	instance->debug_text_queue.clear();
}

void Renderer2D::RenderText(const Font* const font, const float& x, const float& y, const std::string& text)
{
	assert(font);

	Model* const quad = ModelLoader::Get(PRELOADED_MODELS::QUAD);

	const char& c = text.at(0);
	const Glyph& glyph = font->GetGlyph(c);

	GLuint shad = ShaderLoader::Get(PRELOADED_SHADERS::TEXT)->GetProgramID();
	glUseProgram(shad);

	glUniform1i(glGetUniformLocation(shad, "bitmap"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->GetBitmapID());

	const glm::vec2 cell_size(font->GetBitmapWidth() / 16.0f, font->GetBitmapHeight() / 8.0f);

	glUniformMatrix4fv(glGetUniformLocation(shad, "proj_matrix"), 1, GL_FALSE, (const GLfloat*)&instance->proj);
	glUniformMatrix4fv(glGetUniformLocation(shad, "view_matrix"), 1, GL_FALSE, (const GLfloat*)&instance->view);
	glm::mat4 world_matrix = glm::translate(glm::mat4(1.0f), glm::vec3((x - (Glacier::GetWindow().GetWindowWidth() / 2.0f)) + cell_size.x, (y + Glacier::GetWindow().GetWindowHeight() / 2.0f) - cell_size.y * 0.5f, 0));
	world_matrix *= glm::scale(glm::mat4(1.0f), glm::vec3(glyph.size.x, glyph.size.y, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shad, "world_matrix"), 1, GL_FALSE, (const GLfloat*)&world_matrix);

	// c = 65
	const int grid_x = c % 16;
	glUniform4f(glGetUniformLocation(shad, "sprite_data"), grid_x * cell_size.x, cell_size.y * ((c - grid_x) / 16), cell_size.x, cell_size.y);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(quad->GetVBO());
	glDrawElements(GL_TRIANGLES, quad->GetNumTriangles() * 3, GL_UNSIGNED_INT, nullptr);
	glDisable(GL_BLEND);

	glUseProgram(0);
}

void Renderer2D::PrintText(const Font& font, const float& x, const float& y, const std::string& text)
{
	assert(instance && "Instance not created!");
	instance->debug_text_queue.emplace_back(&font, x, y, text);
}
void Renderer2D::PrintText(const Font& font, const float& x, const float& y, const char* const format, ...)
{
	va_list args;
	va_start(args, format);
	char buf[256];
	vsprintf_s(buf, format, args);
	va_end(args);
	PrintText(font, x, y, std::string(buf));
}