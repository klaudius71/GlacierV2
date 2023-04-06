#include "gpch.h"
#include "TextureOpenGL.h"

TextureOpenGL::TextureOpenGL(TextureOpenGL&& o) noexcept
	: Texture(std::move(o)), id(o.id)
{
	o.id = 0;
}
TextureOpenGL& TextureOpenGL::operator=(TextureOpenGL&& o)
{
	Texture::operator=(std::move(o));
	id = o.id;

	o.id = 0;

	return *this;
}
TextureOpenGL::~TextureOpenGL()
{
	if (id != 0)
		glDeleteTextures(1, &id);
}

void TextureOpenGL::Bind(const uint32_t index) const
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture((GLenum)tex_params.type, id);
}
void TextureOpenGL::Unbind() const
{
	glBindTexture((GLenum)tex_params.type, 0);
}

const GLuint TextureOpenGL::GetID() const
{
	return id;
}

void TextureOpenGL::SetTextureWrapS(TEXTURE_WRAP wrap)
{
	Bind(0);
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_WRAP_S, (GLint)wrap);
	Unbind();
}
void TextureOpenGL::SetTextureWrapT(TEXTURE_WRAP wrap)
{
	Bind(0);
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_WRAP_T, (GLint)wrap);
	Unbind();
}
void TextureOpenGL::SetTextureWrapR(TEXTURE_WRAP wrap)
{
	Bind(0);
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_WRAP_R, (GLint)wrap);
	Unbind();
}
void TextureOpenGL::SetTextureMinFilter(TEXTURE_MIN_FILTER filter)
{
	Bind(0);
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_MIN_FILTER, (GLint)filter);
	Unbind();
}
void TextureOpenGL::SetTextureMagFilter(TEXTURE_MAG_FILTER filter)
{
	Bind(0);
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_MAG_FILTER, (GLint)filter);
	Unbind();
}

void TextureOpenGL::load_gpu_data()
{
	// Load the image into the driver
	glGenTextures(1, &id);
	const auto format = channels == 4 ? GL_RGBA : channels == 3 ? GL_RGB : GL_RED;
	Bind(0);
	switch (tex_params.type)
	{
	case TEXTURE_TYPE::REGULAR:
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
		break;
	case TEXTURE_TYPE::CUBE_MAP:
		for (int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img + i * width * height * channels);
		break;
	default:
		assert(false);
		break;
	}

	glTexParameteri((GLenum)tex_params.type, GL_TEXTURE_WRAP_S, (GLint)tex_params.wrap_s);
	glTexParameteri((GLenum)tex_params.type, GL_TEXTURE_WRAP_T, (GLint)tex_params.wrap_t);
	glTexParameteri((GLenum)tex_params.type, GL_TEXTURE_WRAP_R, (GLint)tex_params.wrap_r);
	glTexParameteri((GLenum)tex_params.type, GL_TEXTURE_MIN_FILTER, (GLint)tex_params.min_filter);
	glTexParameteri((GLenum)tex_params.type, GL_TEXTURE_MAG_FILTER, (GLint)tex_params.mag_filter);
}