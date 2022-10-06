#ifndef _FONT
#define _FONT

struct Glyph
{
	glm::i16vec2 size;
	glm::i16vec2 bearing;
	uint32_t advance;
};

class Font
{
public:
	Font(const std::string& file_name, const int& font_size);
	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;
	Font(Font&&) = delete;
	Font& operator=(Font&&) = delete;
	~Font();

	const uint32_t& GetBitmapWidth() const;
	const uint32_t& GetBitmapHeight() const;

private:
	GLuint tex;
	uint32_t bitmap_width;
	uint32_t bitmap_height;
	std::map<char, Glyph> characters;
};

#endif _FONT