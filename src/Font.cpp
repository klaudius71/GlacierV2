#include "gpch.h"
#include "Font.h"

Font::Font(const std::string& file_name, const int& font_size)
	: glyphs(new Glyph[128]), max_height_glyph(glyphs)
{
	FT_Library lib;
	FT_Error error;
	FT_Face face;
	FT_UInt glyph_index;

	error = FT_Init_FreeType(&lib);
	assert(error == FT_Err_Ok);

	error = FT_New_Face(lib, file_name.c_str(), 0, &face);
	assert(!error);

	error = FT_Set_Pixel_Sizes(face, 0, font_size);
	assert(!error);

	bitmap_width = (font_size + 2) * 16;
	bitmap_height = (font_size + 2) * 8;

	uint8_t* buf = new uint8_t[bitmap_height * bitmap_width];
	memset(buf, 0, bitmap_height * bitmap_width);

	int curr_max_under_baseline = 0;
	for (int i = 32; i < 127; i++)
	{
		glyph_index = FT_Get_Char_Index(face, i);

		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		assert(!error);

		const FT_Glyph_Metrics& glyphMetrics = face->glyph->metrics;

		const int glyph_hang = (glyphMetrics.horiBearingY - glyphMetrics.height) / 64;
		if (glyph_hang < curr_max_under_baseline)
			curr_max_under_baseline = glyph_hang;
	}

	for (int i = 0; i < 128; i++)
	{
		glyph_index = FT_Get_Char_Index(face, i);

		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
		assert(!error);

		int x = (i % 16) * (font_size + 2);
		int y = (i / 16) * (font_size + 2);
		x += 1; // 1 pixel padding from the left side of the tile
		y += (font_size + 2) - face->glyph->bitmap_top + curr_max_under_baseline - 1;

		glyphs[i] = Glyph(x - 1, y - 1, face->glyph->bitmap.width + 1, face->glyph->bitmap.rows + 1, face->glyph->bitmap_top, face->glyph->advance.x >> 6 /* advance is given in 1/64 pixels for some reason */);

		if (max_height_glyph->size.y < glyphs[i].size.y)
			max_height_glyph = &glyphs[i];

		// draw the character
		const FT_Bitmap& bitmap = face->glyph->bitmap;
		for (uint32_t xx = 0; xx < bitmap.width; ++xx)
		{
			for (uint32_t yy = 0; yy < bitmap.rows; ++yy)
			{
				const unsigned char& r = bitmap.buffer[yy * (bitmap.width) + xx];
				buf[(y + yy) * bitmap_width + (x + xx)] = r;
			}
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap_width, bitmap_height, 0, GL_RED, GL_UNSIGNED_BYTE, buf);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	delete[] buf;
	FT_Done_Face(face);
	FT_Done_FreeType(lib);
}
Font::~Font()
{
	glDeleteTextures(1, &tex);
	delete[] glyphs;
}

const GLuint& Font::GetBitmapID() const
{
	return tex;
}

const uint32_t& Font::GetBitmapWidth() const
{
	return bitmap_width;
}
const uint32_t& Font::GetBitmapHeight() const
{
	return bitmap_height;
}

const Glyph& Font::GetGlyph(char c) const
{
	assert(c >= 0 && c <= CHAR_MAX);
	return glyphs[c];
}

const Glyph& Font::GetGlyphWithMaxHeight() const
{
	return *max_height_glyph;
}