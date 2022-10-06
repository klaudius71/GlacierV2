#ifndef _FONT
#define _FONT

struct Glyph
{
	glm::uvec2 size;
	glm::uvec2 bearing;
	uint32_t advance;

	Glyph()
		: size(0), bearing(0), advance(0)
	{}
	Glyph(uint32_t size_x, uint32_t size_y, uint32_t bearing_x, uint32_t bearing_y, uint32_t advance)
		: size(size_x, size_y), bearing(bearing_x, bearing_y), advance(advance)
	{}
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

	const GLuint& GetBitmapID() const;

	const uint32_t& GetBitmapWidth() const;
	const uint32_t& GetBitmapHeight() const;

	const Glyph& GetGlyph(char c) const;

private:
	GLuint tex;
	uint32_t bitmap_width;
	uint32_t bitmap_height;
	Glyph* glyphs;
};

#endif _FONT