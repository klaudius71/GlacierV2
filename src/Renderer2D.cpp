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
#include "ConstantBuffer.h"
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
	glyph_data = new VertexTypes::GlyphDataArray;
}
Renderer2D::~Renderer2D()
{
	delete glyph_data;
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
		RenderTextInstanced(entry);

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
	ShaderLoader::GetCamDataConstantBuffer()->UpdateData(devcon, &CamData, sizeof(VertexTypes::CamData));

	// More setup
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
		instance_data_cbuffer->UpdateData(devcon, &InstanceData, sizeof(VertexTypes::InstanceData));

		// Sprite data
		SpriteData.TexelPos = render.texel_origin;
		SpriteData.Size = render.size;
		SpriteData.TexSize = { render.tex->GetWidth(), render.tex->GetHeight() };
		sprite_data_cbuffer->UpdateData(devcon, &SpriteData, sizeof(VertexTypes::SpriteData));

		// Bind texture
		render.tex->Bind(0);

		// Draw
		devcon->DrawIndexed(6, 0, 0);
	}
	DX::DisableBlending();

	for (const auto& entry : debug_text_queue)
		RenderTextInstanced(entry);

	debug_text_queue.clear();
#endif
}

#if GLACIER_OPENGL
void Renderer2D::RenderText(const DebugTextQueueEntry& entry)
{
	// Get the quad model
	const ModelOpenGL* quad = (const ModelOpenGL*)ModelLoader::Get(PRELOADED_MODELS::QUAD);
	// Get the text shader
	auto shad = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::TEXT);
	shad->Bind();

	// Set the bitmap texture
	glUniform1i(shad->GetUniformLocation("bitmap"), 0);
	entry.font->GetTexture()->Bind(0);
	
	// Set the color and projection matrix uniforms
	glUniform4fv(shad->GetUniformLocation("color"), 1, (const GLfloat*)&entry.color);
	glUniformMatrix4fv(shad->GetUniformLocation("proj_matrix"), 1, GL_FALSE, (const GLfloat*)&instance->proj);
	// Get the world_matrix uniform location
	const GLint world_matrix_uniform_loc = shad->GetUniformLocation("world_matrix");
	
	// Set up the current x position and world_matrix
	float xpos = entry.pos.x - (entry.text.cbegin() != entry.text.cend() ? entry.font->GetGlyph(*entry.text.cbegin()).advance * 0.5f : 0.0f);
	glm::mat4 world_matrix(1.0f);

	// Render the text
	glEnable(GL_BLEND);
	glCullFace(GL_FRONT);
	glBindVertexArray(quad->GetVAO());
	const GLint sprite_data_uniform_loc = shad->GetUniformLocation("sprite_data");
	for (auto it = entry.text.cbegin(); it != entry.text.cend(); ++it)
	{
		const Glyph& glyph = entry.font->GetGlyph(*it);

		xpos += glyph.advance * 0.5f;

		world_matrix[0].x = glyph.size.x *  0.5f;
		world_matrix[1].y = glyph.size.y * -0.5f;
		world_matrix[3].x = xpos;
		world_matrix[3].y = entry.pos.y + glyph.size.y * 0.5f - (glyph.size.y - glyph.bearing_y);
		
		glUniformMatrix4fv(world_matrix_uniform_loc, 1, GL_FALSE, (const GLfloat*)&world_matrix);
		glUniform4f(sprite_data_uniform_loc, glyph.bitmap_origin.x, glyph.bitmap_origin.y, glyph.size.x, glyph.size.y);
		glDrawElements(GL_TRIANGLES, quad->GetNumTriangles() * 3, GL_UNSIGNED_INT, nullptr);
		
		xpos += glyph.advance * 0.5f;
	}
	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}
void Renderer2D::RenderTextInstanced(const DebugTextQueueEntry& entry)
{
	Renderer2D& renderer_instance = Instance();

	// Get the quad model
	const ModelOpenGL* quad = (const ModelOpenGL*)ModelLoader::Get(PRELOADED_MODELS::QUAD);
	// Get the text shader
	auto shad = (const ShaderOpenGL*)ShaderLoader::Get(PRELOADED_SHADERS::TEXT_INSTANCED);
	shad->Bind();

	// Set the bitmap texture
	glUniform1i(shad->GetUniformLocation("bitmap"), 0);
	entry.font->GetTexture()->Bind(0);

	// Set the color and projection matrix uniforms
	glUniform4fv(shad->GetUniformLocation("color"), 1, (const GLfloat*)&entry.color);
	glUniformMatrix4fv(shad->GetUniformLocation("proj_matrix"), 1, GL_FALSE, (const GLfloat*)&instance->proj);

	// Get the glyph_data uniform location
	const GLint glyph_data_uniform_loc = shad->GetUniformLocation("glyph_data[0]");
	assert(glyph_data_uniform_loc != -1);

	// Get the world_data uniform location
	const GLint world_data_uniform_loc = shad->GetUniformLocation("world_data[0]");
	assert(world_data_uniform_loc != -1);

	// Set up the current x offset
	float x_offset = !entry.text.empty() ? entry.font->GetGlyph(entry.text.at(0)).advance * -0.5f
															  : 0.0f;

	// Set the world_matrix uniform
	const Glyph& glyph_with_max_height = entry.font->GetGlyphWithMaxHeight();
	const float y_offset = glyph_with_max_height.size.y - glyph_with_max_height.bearing_y;
	const glm::mat4 world_matrix = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(entry.pos.x - x_offset, entry.pos.y - y_offset, 0.0f)), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(shad->GetUniformLocation("world_matrix"), 1, GL_FALSE, (const GLfloat*)&world_matrix);

	// Fill up the instance buffers
	const size_t num_characters = entry.text.size() > MAX_CHARACTERS ? MAX_CHARACTERS : entry.text.size();
	for (int i = 0; i < num_characters; i++)
	{
		const Glyph& glyph = entry.font->GetGlyph(entry.text.at(i));
		x_offset += glyph.advance * 0.5f;

		glm::vec4& glyph_data = (glm::vec4&)renderer_instance.glyph_data->Data[i];
		glyph_data.x = glyph.bitmap_origin.x;
		glyph_data.y = glyph.bitmap_origin.y;
		glyph_data.z = glyph.size.x;
		glyph_data.w = glyph.size.y;

		glm::vec4& world_data = (glm::vec4&)renderer_instance.glyph_data->WorldData[i];
		world_data.x = x_offset;
		world_data.y = glyph.size.y * 0.5f - (glyph.size.y - glyph.bearing_y);
		world_data.z = glyph.size.x * 0.5f;
		world_data.w = glyph.size.y * -0.5f;

		x_offset += glyph.advance * 0.5f;
	}
	glUniform4fv(glyph_data_uniform_loc, (GLsizei)entry.text.size(), (const GLfloat*)renderer_instance.glyph_data->Data);
	glUniform4fv(world_data_uniform_loc, (GLsizei)entry.text.size(), (const GLfloat*)renderer_instance.glyph_data->WorldData);

	// Render the text
	glBindVertexArray(quad->GetVAO());
	glDrawElementsInstanced(GL_TRIANGLES, quad->GetNumTriangles() * 3, GL_UNSIGNED_INT, nullptr, (GLsizei)entry.text.size());
}
#elif GLACIER_DIRECTX
void Renderer2D::RenderText(const DebugTextQueueEntry& text)
{
	// Get the DX variables
	auto devcon = DX::GetDeviceContext();
	auto cam_data_cbuffer = ShaderLoader::GetCamDataConstantBuffer();
	auto instance_data_cbuffer = ShaderLoader::GetInstanceDataConstantBuffer();
	auto sprite_data_cbuffer = ShaderLoader::GetSpriteDataConstantBuffer();
	// Get the quad model
	ModelLoader::Get(PRELOADED_MODELS::QUAD)->Bind();
	// Get the text shader
	ShaderLoader::Get(PRELOADED_SHADERS::TEXT)->Bind();
	// Bind the bitmap texture
	text.font->GetTexture()->Bind(0);

	// Set the camera matrices
	VertexTypes::CamData cam_data{ instance->proj, glm::mat4(1.0f) };
	cam_data_cbuffer->UpdateData(devcon, &cam_data, sizeof(VertexTypes::CamData));

	// Set up the sprite data structure
	VertexTypes::SpriteData sprite_data{ glm::vec2(0.0f), glm::vec2(0.0f), text.color, glm::vec2(text.font->GetBitmapWidth(), text.font->GetBitmapHeight())};

	// Set up the current x position and world_matrix
	float xpos = text.pos.x;
	VertexTypes::InstanceData instance_data{ glm::mat4(1.0f) };

	// Render the text
	DX::EnableBlending();
	DX::EnableFrontFaceCulling();
	for (auto it = text.text.cbegin(); it != text.text.cend(); ++it)
	{
		const Glyph& glyph = text.font->GetGlyph(*it);

		xpos += glyph.advance * 0.5f;
		sprite_data.TexelPos = { glyph.bitmap_origin.x, glyph.bitmap_origin.y };
		sprite_data.Size = { glyph.size.x, glyph.size.y	};
		sprite_data_cbuffer->UpdateData(devcon, &sprite_data, sizeof(VertexTypes::SpriteData));

		instance_data.World[0].x = glyph.size.x * 0.5f;
		instance_data.World[1].y = glyph.size.y * -0.5f;
		instance_data.World[3].x = xpos;
		instance_data.World[3].y = text.pos.y + glyph.size.y * 0.5f - (glyph.size.y - glyph.bearing_y);
		instance_data_cbuffer->UpdateData(devcon, &instance_data, sizeof(VertexTypes::InstanceData));

		devcon->DrawIndexed(6, 0, 0);

		xpos += glyph.advance * 0.5f;
	}
	DX::EnableBackFaceCulling();
	DX::DisableBlending();
}
void Renderer2D::RenderTextInstanced(const DebugTextQueueEntry& entry)
{
	Renderer2D& renderer_instance = Instance();

	// Get the DX variables
	auto devcon = DX::GetDeviceContext();
	auto instance_data_cbuffer = ShaderLoader::GetInstanceDataConstantBuffer();
	auto glyph_data_cbuffer = ShaderLoader::GetGlyphDataConstantBuffer();

	// Bind the quad model
	ModelLoader::Get(PRELOADED_MODELS::QUAD)->Bind();
	// Bind the text shader
	ShaderLoader::Get(PRELOADED_SHADERS::TEXT_INSTANCED)->Bind();

	// Bind the bitmap texture
	entry.font->GetTexture()->Bind(0);

	// Set the camera matrices
	const VertexTypes::CamData cam_data{ instance->proj, glm::mat4(1.0f) };
	ShaderLoader::GetCamDataConstantBuffer()->UpdateData(devcon, &cam_data, sizeof(VertexTypes::CamData));

	// Set the color and texture size
	renderer_instance.glyph_data->Color = entry.color;
	renderer_instance.glyph_data->TexSize = { entry.font->GetBitmapWidth(), entry.font->GetBitmapHeight() };

	// Set up the current x offset
	float x_offset = !entry.text.empty() ? entry.font->GetGlyph(entry.text.at(0)).advance * -0.5f
										 : 0.0f;

	// Set the world_matrix uniform
	const Glyph& glyph_with_max_height = entry.font->GetGlyphWithMaxHeight();
	const float y_offset = glyph_with_max_height.size.y - glyph_with_max_height.bearing_y;
	const glm::mat4 world_matrix = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(entry.pos.x - x_offset, entry.pos.y - y_offset, 0.0f)), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	instance_data_cbuffer->UpdateData(devcon, &world_matrix, sizeof(glm::mat4));

	// Fill up the instance buffers
	const uint32_t num_characters = std::min((int)entry.text.size(), MAX_CHARACTERS);
	for (uint32_t i = 0; i < num_characters; i++)
	{
		const Glyph& glyph = entry.font->GetGlyph(entry.text.at(i));
		x_offset += glyph.advance * 0.5f;

		glm::vec4& glyph_data = (glm::vec4&)renderer_instance.glyph_data->Data[i];
		glyph_data.x = glyph.bitmap_origin.x;
		glyph_data.y = glyph.bitmap_origin.y;
		glyph_data.z = glyph.size.x;
		glyph_data.w = glyph.size.y;

		glm::vec4& world_data = (glm::vec4&)renderer_instance.glyph_data->WorldData[i];
		world_data.x = x_offset;
		world_data.y = glyph.size.y * 0.5f - (glyph.size.y - glyph.bearing_y);
		world_data.z = glyph.size.x * 0.5f;
		world_data.w = glyph.size.y * -0.5f;

		x_offset += glyph.advance * 0.5f;
	}

	glyph_data_cbuffer->UpdateData(devcon, renderer_instance.glyph_data, sizeof(VertexTypes::GlyphDataArray));

	// Render the text
	DX::EnableBlending();
	DX::EnableFrontFaceCulling();
	devcon->DrawIndexedInstanced(6, num_characters, 0, 0, 0);
	DX::EnableBackFaceCulling();
	DX::DisableBlending();
}
#endif

void Renderer2D::PrintText(const Font& font, const float x, const float y, const glm::vec4& color, const std::string& text)
{
	assert(instance && "Instance not created!");
	assert(text.length() < MAX_CHARACTERS);
	instance->debug_text_queue.emplace_back(&font, x, y, text, color);
}
void Renderer2D::PrintText(const Font& font, const float x, const float y, const glm::vec4& color, const char* const format, ...)
{
	va_list args;
	va_start(args, format);
	char buf[256];
	vsprintf_s(buf, format, args);
	va_end(args);
	PrintText(font, x, y, color, std::string(buf));
}