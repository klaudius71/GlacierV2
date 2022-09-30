#include "gpch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& file_name)
	: id(0xFFFFFFFF), type(TEXTURE_TYPE::REGULAR)
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
Texture::Texture(const std::array<std::string, 6>& file_paths)
	: id(0xFFFFFFFF), type(TEXTURE_TYPE::CUBE_MAP)
{
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
Texture::Texture(Texture&& o) noexcept
	: id(o.id), width(o.width), height(o.height), channels(o.channels), type(o.type), img(o.img)
{
	o.img = nullptr;
}
Texture& Texture::operator=(Texture&& o)
{
	id = o.id;
	width = o.width;
	height = o.height;
	channels = o.channels;
	type = o.type;
	img = o.img;
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
const TEXTURE_TYPE& Texture::GetTextureType() const
{
	return type;
}

void Texture::load_GPU_data(Texture& tex)
{
	// Load the image into the driver
	glGenTextures(1, &tex.id);
	const auto format = tex.channels == 3 ? GL_RGB : GL_RGBA;
	switch (tex.type)
	{
	case TEXTURE_TYPE::REGULAR:
		glBindTexture(GL_TEXTURE_2D, tex.id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, tex.img);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
		glBindTexture(GL_TEXTURE_2D, 0);
		break;
	case TEXTURE_TYPE::CUBE_MAP:
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex.id);
		for (int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, tex.img + i * tex.width * tex.height * tex.channels);
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	default:
		assert(false);
		break;
	}
	
}