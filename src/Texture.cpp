#include "gpch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& file_name, const TextureParameters& tex_params)
	: id(0), tex_params(tex_params), file_path(file_name)
{
	// Vertically flips all loaded textures
	stbi_set_flip_vertically_on_load(true);
	// Load a texture into memory as bytes
	img = stbi_load(file_name.c_str(), &width, &height, &channels, 0);
	if (!img)
	{
		printf("Failed to load image %s\n", file_name.c_str());
		printf("Loading default image...\n");
		img = stbi_load("default.tga", &width, &height, &channels, 0);
	}
}
Texture::Texture(const std::array<std::string, 6>& file_paths, const TextureParameters& tex_params)
	: id(0), tex_params(tex_params)
{
	this->tex_params.type = TEXTURE_TYPE::CUBE_MAP;

	// Vertically flips all loaded textures
	stbi_set_flip_vertically_on_load(true);

	uint8_t* imgs[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	int width_height_channels[3 * 6];
	for (int i = 0; i < 6; i++)
		imgs[i] = stbi_load(file_paths[i].c_str(), &width_height_channels[i * 3], &width_height_channels[i * 3 + 1], &width_height_channels[i * 3 + 2], 0);

	assert(imgs[0] && imgs[1] && imgs[2] && imgs[3] && imgs[4] && imgs[5]);
	assert(width_height_channels[0] == width_height_channels[1] && width_height_channels[3] == width_height_channels[4] && width_height_channels[6] == width_height_channels[7] && width_height_channels[9] == width_height_channels[10] && width_height_channels[12] == width_height_channels[13]);
	assert(width_height_channels[0] == width_height_channels[3] && width_height_channels[3] == width_height_channels[6] && width_height_channels[6] == width_height_channels[9] && width_height_channels[9] == width_height_channels[12] && width_height_channels[12] == width_height_channels[15]);

	width = width_height_channels[0];
	height = width_height_channels[1];
	channels = width_height_channels[2];

	const size_t indiv_img_buffer_size = width * height * channels;
	img = (uint8_t*)malloc(indiv_img_buffer_size * 6);
	for (int i = 0; i < 6; i++)
	{
		memcpy(img + i * indiv_img_buffer_size, imgs[i], indiv_img_buffer_size);
		stbi_image_free(imgs[i]);
	}
}
Texture::Texture(const glm::vec4& color)
	: id(0), tex_params(TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR, TEXTURE_WRAP::CLAMP_TO_EDGE, TEXTURE_WRAP::CLAMP_TO_EDGE, TEXTURE_WRAP::CLAMP_TO_EDGE)
{
	this->tex_params.type = TEXTURE_TYPE::REGULAR;
	width = 1;
	height = 1;
	channels = 4;
	img = new uint8_t[4];
	glm::ivec4 color_bytes = color * 255.0f;
	for (int i = 0; i < 4; i++)
		img[i] = (uint8_t)color_bytes[i];
}

Texture::Texture(Texture&& o) noexcept
	: id(o.id), width(o.width), height(o.height), channels(o.channels), tex_params(o.tex_params), img(o.img), file_path(std::move(o.file_path))
{
	o.id = 0;
	o.width = 0;
	o.height = 0;
	o.channels = 0;
	memset(&o.tex_params, 0, sizeof(TextureParameters));
	o.img = nullptr;
}
Texture& Texture::operator=(Texture&& o)
{
	id = o.id;
	width = o.width;
	height = o.height;
	channels = o.channels;
	tex_params = o.tex_params;
	img = o.img;
	file_path = std::move(o.file_path);

	o.id = 0;
	o.width = 0;
	o.height = 0;
	o.channels = 0;
	memset(&o.tex_params, 0, sizeof(TextureParameters));
	o.img = nullptr;

	return *this;
}
Texture::~Texture()
{
	if (img != nullptr)
	{
		// Frees the memory that stb_image used to hold the image
		free(img);
		glDeleteTextures(1, &id);
	}
}

void Texture::Bind() const
{
	glBindTexture((GLenum)tex_params.type, id);
}
void Texture::Unbind() const
{
	glBindTexture((GLenum)tex_params.type, 0);
}

const GLuint& Texture::GetID() const
{
	return id;
}

const int& Texture::GetWidth() const
{
	return width;
}
const int& Texture::GetHeight() const
{
	return height;
}
const int& Texture::GetNumChannels() const
{
	return channels;
}
const TextureParameters& Texture::GetTextureParameters() const
{
	return tex_params;
}
const std::string& Texture::GetFilePath() const
{
	return file_path;
}

void Texture::SetTextureWrapS(TEXTURE_WRAP wrap)
{
	Bind();
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_WRAP_S, (GLint)wrap);
	Unbind();
}
void Texture::SetTextureWrapT(TEXTURE_WRAP wrap)
{
	Bind();
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_WRAP_T, (GLint)wrap);
	Unbind();
}
void Texture::SetTextureWrapR(TEXTURE_WRAP wrap)
{
	Bind();
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_WRAP_R, (GLint)wrap);
	Unbind();
}
void Texture::SetTextureMinFilter(TEXTURE_MIN_FILTER filter)
{
	Bind();
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_MIN_FILTER, (GLint)filter);
	Unbind();
}
void Texture::SetTextureMagFilter(TEXTURE_MAG_FILTER filter)
{
	Bind();
	glTexParameteri((GLuint)tex_params.type, GL_TEXTURE_MAG_FILTER, (GLint)filter);
	Unbind();
}

void Texture::load_gpu_data()
{
	// Load the image into the driver
	glGenTextures(1, &id);
	const auto format = channels == 3 ? GL_RGB : GL_RGBA;
	Bind();
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