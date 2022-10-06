#ifndef _RENDERER_2D
#define _RENDERER_2D

#include "Components.h"

#define RESERVED_DEBUG_TEXT_QUERIES 20

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

		DebugTextQueueEntry(const Font* const font, const float& x, const float& y, const std::string& text)
			: font(font), pos(x, y), text(text)
		{}
	};
	std::vector<DebugTextQueueEntry> debug_text_queue;

	static void Initialize();
	static void UpdateScreenSize(const int& width, const int& height);
	static void Terminate();

	static void RenderComponents(Scene& scn);

	static void RenderText(const Font* const font, const float& x, const float& y, const std::string& text);

	friend class Renderer2DAtt;

public:
	static void PrintText(const Font& font, const float& x, const float& y, const std::string& text);
	static void PrintText(const Font& font, const float& x, const float& y, const char* const format, ...);
};

#endif _RENDERER_2D