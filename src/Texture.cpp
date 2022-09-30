#include "gpch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& file_name)
	: id(0xFFFFFFFF)
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
Texture::Texture(Texture&& o) noexcept
	: id(o.id), width(o.width), height(o.height), channels(o.channels), img(o.img)
{
	o.img = nullptr;
}
Texture& Texture::operator=(Texture&& o)
{
	id = o.id;
	width = o.width;
	height = o.height;
	channels = o.channels;
	img = o.img;
	o.img = nullptr;
	return *this;
}
Texture::~Texture()
{
	if (img != nullptr)
	{
		// Frees the memory that stb_image used to hold the image
		stbi_image_free(img);
		glDeleteTextures(1, &id);
	}
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
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

void Texture::load_GPU_data(Texture& tex)
{
	// Load the image into the driver
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	const auto format = tex.channels == 3 ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, tex.img);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
	glBindTexture(GL_TEXTURE_2D, 0);
}