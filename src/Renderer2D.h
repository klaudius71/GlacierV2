#ifndef _RENDERER_2D
#define _RENDERER_2D

#define RESERVED_DEBUG_TEXT_QUERIES 20
#define MAX_CHARACTERS 100

class Scene;
class Font;

class Renderer2D
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

	glm::vec2 anchors[9];

	static void Initialize();
	static void UpdateViewportSize(const int& width, const int& height);
	static void Terminate();

	void renderComponents(Scene& scn);
	static void RenderComponents(Scene& scn) { assert(instance && "Renderer2D not initialized!"); instance->renderComponents(scn); }

	static void RenderText(const Font* const font, const float& x, const float& y, const glm::vec4& color, const std::string& text);
	static void RenderTextInstanced(const Font* const font, const float& x, const float& y, const glm::vec4& color, const std::string& text);

	friend class Renderer2DAtt;

public:
	static void PrintText(const Font& font, const float& x, const float& y, const glm::vec4& color, const std::string& text);
	static void PrintText(const Font& font, const float& x, const float& y, const glm::vec4& color, const char* const format, ...);

	static const glm::mat4& GetCurrentProjectionMatrix() { assert(instance); return instance->proj; }
};

#endif _RENDERER_2D