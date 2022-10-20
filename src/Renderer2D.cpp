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
	proj = glm::ortho(0.0f, (float)window.GetWindowWidth(), 0.0f, (float)window.GetWindowHeight());
	view = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	debug_text_queue.reserve(RESERVED_DEBUG_TEXT_QUERIES);
	uniform_glyph_data.reserve(MAX_CHARACTERS);
	uniform_world_data.reserve(MAX_CHARACTERS);
	for (int i = 0; i < MAX_CHARACTERS; i++)
	{
		uniform_glyph_data.emplace_back(0.0f);
		uniform_world_data.emplace_back(0.0f);
	}
}

void Renderer2D::Initialize()
{
	instance = new Renderer2D;
}
void Renderer2D::UpdateScreenSize(const int& width, const int& height)
{
	instance->proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
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
		RenderTextInstanced(entry.font, entry.pos.x, entry.pos.y, entry.color, entry.text);

	instance->debug_text_queue.clear();
}

void Renderer2D::RenderText(const Font* const font, const float& x, const float& y, const glm::vec4& color, const std::string& text)
{
	// Get the quad model
	Model* const quad = ModelLoader::Get(PRELOADED_MODELS::QUAD);
	// Get the text shader
	GLuint shad = ShaderLoader::Get(PRELOADED_SHADERS::TEXT)->GetProgramID();
	glUseProgram(shad);

	// Set the bitmap texture
	glUniform1i(glGetUniformLocation(shad, "bitmap"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->GetBitmapID());
	
	// Set the color and projection matrix uniforms
	glUniform4fv(glGetUniformLocation(shad, "color"), 1, (const GLfloat*)&color);
	glUniformMatrix4fv(glGetUniformLocation(shad, "proj_matrix"), 1, GL_FALSE, (const GLfloat*)&instance->proj);
	// Get the world_matrix uniform location
	const GLint world_matrix_uniform_loc = glGetUniformLocation(shad, "world_matrix");
	
	// Set up the current x position and world_matrix
	float xpos = x - (text.cbegin() != text.cend() ? font->GetGlyph(*text.cbegin()).advance * 0.5f : 0.0f);
	glm::mat4 world_matrix(1.0f);

	// Render the text
	glEnable(GL_BLEND);
	glCullFace(GL_FRONT);
	glBindVertexArray(quad->GetVAO());
	const GLint sprite_data_uniform_loc = glGetUniformLocation(shad, "sprite_data");
	for (auto it = text.cbegin(); it != text.cend(); ++it)
	{
		const Glyph& glyph = font->GetGlyph(*it);

		xpos += glyph.advance * 0.5f;

		world_matrix[0].x = glyph.size.x *  0.5f;
		world_matrix[1].y = glyph.size.y * -0.5f;
		world_matrix[3].x = xpos;
		world_matrix[3].y = y + glyph.size.y * 0.5f - (glyph.size.y - glyph.bearing_y);
		
		glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&world_matrix);
		glUniform4f(sprite_data_uniform_loc, glyph.bitmap_origin.x, glyph.bitmap_origin.y, glyph.size.x, glyph.size.y);
		glDrawElements(GL_TRIANGLES, quad->GetNumTriangles() * 3, GL_UNSIGNED_INT, nullptr);
		
		xpos += glyph.advance * 0.5f;
	}
	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}
void Renderer2D::RenderTextInstanced(const Font* const font, const float& x, const float& y, const glm::vec4& color, const std::string& text)
{
	Renderer2D& renderer_instance = *instance;

	// Get the quad model
	Model* const quad = ModelLoader::Get(PRELOADED_MODELS::QUAD);
	// Get the text shader
	GLuint shad = ShaderLoader::Get(PRELOADED_SHADERS::TEXT_INSTANCED)->GetProgramID();
	glUseProgram(shad);

	// Set the bitmap texture
	glUniform1i(glGetUniformLocation(shad, "bitmap"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->GetBitmapID());

	// Set the color and projection matrix uniforms
	glUniform4fv(glGetUniformLocation(shad, "color"), 1, (const GLfloat*)&color);
	glUniformMatrix4fv(glGetUniformLocation(shad, "proj_matrix"), 1, GL_FALSE, (const GLfloat*)&instance->proj);

	// Get the glyph_data uniform location
	const GLint glyph_data_uniform_loc = glGetUniformLocation(shad, "glyph_data[0]");
	assert(glyph_data_uniform_loc != -1);

	// Get the world_data uniform location
	const GLint world_data_uniform_loc = glGetUniformLocation(shad, "world_data[0]");
	assert(world_data_uniform_loc != -1);

	// Set up the current x offset
	float x_offset = text.cbegin() != text.cend() ? font->GetGlyph(text.at(0)).advance * -0.5f : 0.0f;

	// Set the world_matrix uniform
	const Glyph& glyph_with_max_height = font->GetGlyphWithMaxHeight();
	const float y_offset = glyph_with_max_height.size.y - glyph_with_max_height.bearing_y;
	const glm::mat4 world_matrix = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(x - x_offset, y - y_offset, 0.0f)), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(shad, "world_matrix"), 1, GL_FALSE, (const GLfloat*)&world_matrix);

	// Fill up the instance buffers
	for (int i = 0; i < text.size(); i++)
	{
		const Glyph& glyph = font->GetGlyph(text.at(i));
		x_offset += glyph.advance * 0.5f;

		glm::vec4& glyph_data = renderer_instance.uniform_glyph_data[i];
		glyph_data.x = glyph.bitmap_origin.x;
		glyph_data.y = glyph.bitmap_origin.y;
		glyph_data.z = glyph.size.x;
		glyph_data.w = glyph.size.y;

		glm::vec4& world_data = renderer_instance.uniform_world_data[i];
		world_data.x = x_offset;
		world_data.y = glyph.size.y * 0.5f - (glyph.size.y - glyph.bearing_y);
		world_data.z = glyph.size.x * 0.5f;
		world_data.w = glyph.size.y * -0.5f;

		x_offset += glyph.advance * 0.5f;
	}
	glUniform4fv(glyph_data_uniform_loc, (GLsizei)text.size(), (const GLfloat*)renderer_instance.uniform_glyph_data.data());
	glUniform4fv(world_data_uniform_loc, (GLsizei)text.size(), (const GLfloat*)renderer_instance.uniform_world_data.data());

	// Render the text
	glEnable(GL_BLEND);
	glCullFace(GL_FRONT);
	glBindVertexArray(quad->GetVAO());
	glDrawElementsInstanced(GL_TRIANGLES, quad->GetNumTriangles() * 3, GL_UNSIGNED_INT, nullptr, (GLsizei)text.size());
	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}

void Renderer2D::PrintText(const Font& font, const float& x, const float& y, const glm::vec4& color, const std::string& text)
{
	assert(instance && "Instance not created!");
	assert(text.length() < MAX_CHARACTERS);
	instance->debug_text_queue.emplace_back(&font, x, y, text, color);
}
void Renderer2D::PrintText(const Font& font, const float& x, const float& y, const glm::vec4& color, const char* const format, ...)
{
	va_list args;
	va_start(args, format);
	char buf[256];
	vsprintf_s(buf, format, args);
	va_end(args);
	PrintText(font, x, y, color, std::string(buf));
}