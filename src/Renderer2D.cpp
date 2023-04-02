#include "gpch.h"
#include "Renderer2D.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Components.h"
#include "Application.h"
#include "Window.h"
#include "Scene.h"
#include "ModelLoader.h"
#include "Font.h"
#include "ShaderLoader.h"
#include "Logger.h"
#if GLACIER_OPENGL
#include "ShaderOpenGL.h"
#include "TextureOpenGL.h"
#elif GLACIER_DIRECTX
#include "DX.h"
#include "TextureDirectX.h"
#endif

Renderer2D* Renderer2D::instance = nullptr;

Renderer2D::Renderer2D()
	: anchors()
{
	const auto& window = Glacier::GetWindow();
	const int& width = window.GetWindowWidth();
	const int& height = window.GetWindowHeight();
	proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
	view = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	anchors[0] = { 0.0f, height };
	anchors[1] = { width * 0.5f, height };
	anchors[2] = { width, height };
	anchors[3] = { 0.0f, height * 0.5f };
	anchors[4] = { width * 0.5f, height * 0.5f };
	anchors[5] = { width, height * 0.5f };
	anchors[6] = { 0.0f, 0.0f };
	anchors[7] = { width * 0.5f, 0.0f };
	anchors[8] = { width, 0.0f };

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
	assert(instance == nullptr);
	instance = new Renderer2D;
}
void Renderer2D::UpdateViewportSize(const int& width, const int& height)
{
	Renderer2D& inst = Instance();

	inst.proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

	inst.anchors[0] = { 0.0f, height };
	inst.anchors[1] = { width * 0.5f, height };
	inst.anchors[2] = { width, height };
	inst.anchors[3] = { 0.0f, height * 0.5f };
	inst.anchors[4] = { width * 0.5f, height * 0.5f };
	inst.anchors[5] = { width, height * 0.5f };
	inst.anchors[6] = { 0.0f, 0.0f };
	inst.anchors[7] = { width * 0.5f, 0.0f };
	inst.anchors[8] = { width, 0.0f };
}
void Renderer2D::Terminate()
{
	delete instance;
	instance = nullptr;
}

void Renderer2D::renderComponents(Scene& scn)
{
	GLACIER_LOG_FUNC_TIMER("2d");

#if GLACIER_OPENGL
	const Framebuffer& framebuffer = Renderer::GetMainFramebuffer();
	const glm::ivec2& viewport_size = framebuffer.GetSize();
	glViewport(0, 0, viewport_size.x, viewport_size.y);
	framebuffer.Bind();

	auto shad = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::SPRITE);
	shad->Bind();
	glUniformMatrix4fv(shad->GetUniformLocation("proj_matrix"), 1, GL_FALSE, (const GLfloat*)&instance->proj);
	glUniform1i(shad->GetUniformLocation("sprite_texture"), 0);
	const GLint sprite_data_uniform_loc = shad->GetUniformLocation("sprite_data");
	const GLint world_matrix_uniform_loc = shad->GetUniformLocation("world_matrix");
	glm::mat4 curr_world_matrix;

	const GLuint quad = ((const ModelOpenGL*)ModelLoader::Get(PRELOADED_MODELS::QUAD))->GetVAO();
	glBindVertexArray(quad);
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	auto group = scn.GetRegistry().group<SpriteComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, render, transform] : group.each())
	{
		curr_world_matrix = glm::translate(glm::mat4(1.0f), transform.pos);
		curr_world_matrix *= glm::rotate(glm::mat4(1.0f), transform.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		curr_world_matrix *= glm::scale(glm::mat4(1.0f), transform.scl);
		(glm::vec2&)curr_world_matrix[3] += anchors[(uint32_t)render.anchor];
		glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, glm::value_ptr(curr_world_matrix));
		glUniform4fv(sprite_data_uniform_loc, 1, (const GLfloat*)&render.data);
		glBindTexture(GL_TEXTURE_2D, render.tex_id);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	glCullFace(GL_FRONT);
	for (const auto& entry : debug_text_queue)
		RenderTextInstanced(entry.font, entry.pos.x, entry.pos.y, entry.color, entry.text);

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);

	debug_text_queue.clear();

	const Window& window = Glacier::GetWindow();
	glViewport(0, 0, window.GetWindowWidth(), window.GetWindowHeight());
	framebuffer.Unbind();
#elif GLACIER_DIRECTX
	auto devcon = DX::GetDeviceContext();
	auto instance_data_cbuffer = ShaderLoader::GetInstanceDataConstantBuffer();
	auto sprite_data_cbuffer = ShaderLoader::GetSpriteDataConstantBuffer();
	
	auto shad = ShaderLoader::Get(PRELOADED_SHADERS::SPRITE);
	shad->Bind();

	// Set the camera matrices
	VertexTypes::CamData CamData{ instance->proj, glm::mat4(1.0f) };
	devcon->UpdateSubresource(ShaderLoader::GetCamDataConstantBuffer(), 0, nullptr, &CamData, 0, 0);

	// More setup
	devcon->VSSetConstantBuffers(2, 1, &sprite_data_cbuffer);
	devcon->PSSetConstantBuffers(2, 1, &sprite_data_cbuffer);
	ModelLoader::Get(PRELOADED_MODELS::QUAD)->Bind();
	VertexTypes::InstanceData InstanceData;
	VertexTypes::SpriteData SpriteData{ glm::vec2(0.0f), glm::vec2(0.0f), Colors::White, glm::vec2(0.0f) };

	DX::EnableBlending();
	auto group = scn.GetRegistry().group<SpriteComponent>(entt::get<TransformComponent>);
	for (auto&& [entity, render, transform] : group.each())
	{
		// World matrix
		InstanceData.World = glm::translate(glm::mat4(1.0f), transform.pos);
		InstanceData.World *= glm::rotate(glm::mat4(1.0f), transform.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		InstanceData.World *= glm::scale(glm::mat4(1.0f), transform.scl);
		(glm::vec2&)InstanceData.World[3] += anchors[(uint32_t)render.anchor];
		devcon->UpdateSubresource(instance_data_cbuffer, 0, nullptr, &InstanceData, 0, 0);

		// Sprite data
		SpriteData.TexelPos = render.texel_origin;
		SpriteData.Size = render.size;
		SpriteData.TexSize = { render.tex->GetWidth(), render.tex->GetHeight() };
		devcon->UpdateSubresource(sprite_data_cbuffer, 0, nullptr, &SpriteData, 0, 0);

		// Bind texture
		render.tex->Bind();

		// Draw
		devcon->DrawIndexed(6, 0, 0);
	}
	DX::DisableBlending();

	for (const auto& entry : debug_text_queue)
		RenderText(entry.font, entry.pos.x, entry.pos.y, entry.color, entry.text);

	debug_text_queue.clear();
#endif
}

#if GLACIER_OPENGL
void Renderer2D::RenderText(const Font* const font, const float& x, const float& y, const glm::vec4& color, const std::string& text)
{
	// Get the quad model
	const ModelOpenGL* quad = (const ModelOpenGL*)ModelLoader::Get(PRELOADED_MODELS::QUAD);
	// Get the text shader
	auto shad = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::TEXT);
	shad->Bind();

	// Set the bitmap texture
	glUniform1i(shad->GetUniformLocation("bitmap"), 0);
	glActiveTexture(GL_TEXTURE0);
	font->GetTexture()->Bind();
	
	// Set the color and projection matrix uniforms
	glUniform4fv(shad->GetUniformLocation("color"), 1, (const GLfloat*)&color);
	glUniformMatrix4fv(shad->GetUniformLocation("proj_matrix"), 1, GL_FALSE, (const GLfloat*)&instance->proj);
	// Get the world_matrix uniform location
	const GLint world_matrix_uniform_loc = shad->GetUniformLocation("world_matrix");
	
	// Set up the current x position and world_matrix
	float xpos = x - (text.cbegin() != text.cend() ? font->GetGlyph(*text.cbegin()).advance * 0.5f : 0.0f);
	glm::mat4 world_matrix(1.0f);

	// Render the text
	glEnable(GL_BLEND);
	glCullFace(GL_FRONT);
	glBindVertexArray(quad->GetVAO());
	const GLint sprite_data_uniform_loc = shad->GetUniformLocation("sprite_data");
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
	Renderer2D& renderer_instance = Instance();

	// Get the quad model
	const ModelOpenGL* quad = (const ModelOpenGL*)ModelLoader::Get(PRELOADED_MODELS::QUAD);
	// Get the text shader
	auto shad = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::TEXT_INSTANCED);
	shad->Bind();

	// Set the bitmap texture
	glUniform1i(shad->GetUniformLocation("bitmap"), 0);
	glActiveTexture(GL_TEXTURE0);
	font->GetTexture()->Bind();

	// Set the color and projection matrix uniforms
	glUniform4fv(shad->GetUniformLocation("color"), 1, (const GLfloat*)&color);
	glUniformMatrix4fv(shad->GetUniformLocation("proj_matrix"), 1, GL_FALSE, (const GLfloat*)&instance->proj);

	// Get the glyph_data uniform location
	const GLint glyph_data_uniform_loc = shad->GetUniformLocation("glyph_data[0]");
	assert(glyph_data_uniform_loc != -1);

	// Get the world_data uniform location
	const GLint world_data_uniform_loc = shad->GetUniformLocation("world_data[0]");
	assert(world_data_uniform_loc != -1);

	// Set up the current x offset
	float x_offset = text.cbegin() != text.cend() ? font->GetGlyph(text.at(0)).advance * -0.5f : 0.0f;

	// Set the world_matrix uniform
	const Glyph& glyph_with_max_height = font->GetGlyphWithMaxHeight();
	const float y_offset = glyph_with_max_height.size.y - glyph_with_max_height.bearing_y;
	const glm::mat4 world_matrix = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(x - x_offset, y - y_offset, 0.0f)), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(shad->GetUniformLocation("world_matrix"), 1, GL_FALSE, (const GLfloat*)&world_matrix);

	// Fill up the instance buffers
	const size_t num_characters = text.size() > 100 ? 100 : text.size();
	for (int i = 0; i < num_characters; i++)
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
	glBindVertexArray(quad->GetVAO());
	glDrawElementsInstanced(GL_TRIANGLES, quad->GetNumTriangles() * 3, GL_UNSIGNED_INT, nullptr, (GLsizei)text.size());
}
#elif GLACIER_DIRECTX
void Renderer2D::RenderText(const Font* const font, const float& x, const float& y, const glm::vec4& color, const std::string& text)
{
	// Get the DX variables
	auto devcon = DX::GetDeviceContext();
	auto cam_data_cbuffer = ShaderLoader::GetCamDataConstantBuffer();
	auto instance_data_cbuffer = ShaderLoader::GetInstanceDataConstantBuffer();
	auto sprite_data_cbuffer = ShaderLoader::GetSpriteDataConstantBuffer();
	// Get the quad model
	const Model* quad = ModelLoader::Get(PRELOADED_MODELS::QUAD);
	quad->Bind();
	// Get the text shader
	auto shad = ShaderLoader::Get(PRELOADED_SHADERS::TEXT);
	shad->Bind();
	// Bind the bitmap texture
	font->GetTexture()->Bind();

	// Set the camera matrices
	VertexTypes::CamData cam_data{ instance->proj, glm::mat4(1.0f) };
	devcon->UpdateSubresource(cam_data_cbuffer, 0, nullptr, &cam_data, 0, 0);

	// Set up the sprite data structure
	VertexTypes::SpriteData sprite_data{ glm::vec2(0.0f), glm::vec2(0.0f), color, glm::vec2(font->GetBitmapWidth(), font->GetBitmapHeight())};
	devcon->VSSetConstantBuffers(2, 1, &sprite_data_cbuffer);
	devcon->PSSetConstantBuffers(2, 1, &sprite_data_cbuffer);

	// Set up the current x position and world_matrix
	float xpos = x;
	VertexTypes::InstanceData instance_data{ glm::mat4(1.0f) };

	// Render the text
	DX::EnableBlending();
	DX::EnableFrontFaceCulling();
	for (auto it = text.cbegin(); it != text.cend(); ++it)
	{
		const Glyph& glyph = font->GetGlyph(*it);

		xpos += glyph.advance * 0.5f;
		sprite_data.TexelPos = { glyph.bitmap_origin.x, glyph.bitmap_origin.y };
		sprite_data.Size = { glyph.size.x, glyph.size.y	};
		devcon->UpdateSubresource(sprite_data_cbuffer, 0, nullptr, &sprite_data, 0, 0);

		instance_data.World[0].x = glyph.size.x * 0.5f;
		instance_data.World[1].y = glyph.size.y * -0.5f;
		instance_data.World[3].x = xpos;
		instance_data.World[3].y = y + glyph.size.y * 0.5f - (glyph.size.y - glyph.bearing_y);
		devcon->UpdateSubresource(instance_data_cbuffer, 0, nullptr, &instance_data, 0, 0);

		devcon->DrawIndexed(quad->GetNumTriangles() * 3, 0, 0);

		xpos += glyph.advance * 0.5f;
	}
	DX::EnableBackFaceCulling();
	DX::DisableBlending();
}
#endif

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