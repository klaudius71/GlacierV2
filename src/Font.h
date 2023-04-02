#ifndef _FONT
#define _FONT

#include "GlacierCore.h"

struct GLACIER_API Glyph
{
	glm::vec2 bitmap_origin;
	glm::vec2 size;
	float bearing_y;
	float advance;

	Glyph()
		: bitmap_origin(0), size(0), bearing_y(0), advance(0)
	{}
	Glyph(uint32_t bitmap_origin_x, uint32_t bitmap_origin_y, uint32_t size_x, uint32_t size_y, int bearing_y, uint32_t advance)
		: bitmap_origin((float)bitmap_origin_x, (float)bitmap_origin_y), size((float)size_x, (float)size_y), bearing_y((float)bearing_y), advance((float)advance)
	{}
};

class Texture;

class GLACIER_API Font
{
public:
	Font(const std::string& file_name, const int& font_size);
	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;
	Font(Font&&) = delete;
	Font& operator=(Font&&) = delete;
	~Font();

	const Texture* GetTexture() const;

	const uint32_t GetBitmapWidth() const;
	const uint32_t GetBitmapHeight() const;

	const Glyph& GetGlyph(char c) const;

	const Glyph& GetGlyphWithMaxHeight() const;

private:
	const Texture* tex;
	uint32_t bitmap_width;
	uint32_t bitmap_height;
	Glyph* glyphs;
	Glyph* max_height_glyph;
};

#endif _FONT