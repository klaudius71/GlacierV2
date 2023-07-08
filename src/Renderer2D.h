#ifndef _RENDERER_2D
#define _RENDERER_2D

#include "GlacierCore.h"

constexpr auto RESERVED_DEBUG_TEXT_QUERIES = 20;
constexpr auto MAX_CHARACTERS = 128;

class Scene;
class Font;
struct GlyphDataArray;

class GLACIER_API Renderer2D
{
public:
	enum class SCREEN_ANCHOR : uint32_t
	{
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		MIDDLE_LEFT,
		MIDDLE_CENTER,
		MIDDLE_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT
	};

private:
	static Renderer2D* instance;
	static Renderer2D& Instance()
	{
		assert(instance != nullptr);
		return *instance;
	}

	Renderer2D();
	~Renderer2D();

	glm::mat4 proj;
	glm::mat4 view;

	struct DebugTextQueueEntry
	{
		const Font* font;
		glm::vec2 pos;
		std::string text;
		glm::vec4 color;

		DebugTextQueueEntry(const Font* const font, float x, float y, const std::string& text, const glm::vec4& color)
			: font(font), pos(x, y), text(text), color(color)
		{}
	};
	std::vector<DebugTextQueueEntry> debug_text_queue;

	VertexTypes::GlyphDataArray* glyph_data;

	glm::vec2 anchors[9];

	static void Initialize();
	static void UpdateViewportSize(const int& width, const int& height);
	static void Terminate();

	void renderComponents(Scene& scn);
	static void RenderComponents(Scene& scn) { assert(instance && "Renderer2D not initialized!"); instance->renderComponents(scn); }

	static void RenderText(const DebugTextQueueEntry& entry);
	static void RenderTextInstanced(const DebugTextQueueEntry& entry);

	friend class Renderer2DAtt;

public:
	static void PrintText(const Font& font, const float x, const float y, const glm::vec4& color, const std::string& text);
	static void PrintText(const Font& font, const float x, const float y, const glm::vec4& color, const char* const format, ...);

	static const glm::mat4& GetCurrentProjectionMatrix() { assert(instance); return instance->proj; }
};

#endif _RENDERER_2D