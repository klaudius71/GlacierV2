#ifndef _RENDERER_2D
#define _RENDERER_2D

#include "Components.h"

#define RESERVED_DEBUG_TEXT_QUERIES 20
#define MAX_CHARACTERS 100

class Scene;
class Font;

class Renderer2D
{
private:
	static Renderer2D* instance;
	Renderer2D();
	~Renderer2D() = default;

	glm::mat4 proj;
	glm::mat4 view;

	struct DebugTextQueueEntry
	{
		const Font* font;
		glm::vec2 pos;
		std::string text;
		glm::vec4 color;

		DebugTextQueueEntry(const Font* const font, const float& x, const float& y, const std::string& text, const glm::vec4& color)
			: font(font), pos(x, y), text(text), color(color)
		{}
	};
	std::vector<DebugTextQueueEntry> debug_text_queue;

	std::vector<glm::vec4> uniform_glyph_data;
	std::vector<glm::vec4> uniform_world_data;

	static void Initialize();
	static void UpdateViewportSize(const int& width, const int& height);
	static void Terminate();

	static void RenderComponents(Scene& scn);

	static void RenderText(const Font* const font, const float& x, const float& y, const glm::vec4& color, const std::string& text);
	static void RenderTextInstanced(const Font* const font, const float& x, const float& y, const glm::vec4& color, const std::string& text);

	friend class Renderer2DAtt;

public:
	static void PrintText(const Font& font, const float& x, const float& y, const glm::vec4& color, const std::string& text);
	static void PrintText(const Font& font, const float& x, const float& y, const glm::vec4& color, const char* const format, ...);
};

#endif _RENDERER_2D