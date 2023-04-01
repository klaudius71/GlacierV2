#ifndef _TEXTURE
#define _TEXTURE

#include "GlacierCore.h"

enum class TEXTURE_WRAP : uint16_t
{
	NONE = 0,
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, 
	CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER, 
	MIRRORED_REPEAT = GL_MIRRORED_REPEAT, 
	REPEAT = GL_REPEAT, 
	MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE
};
enum class TEXTURE_MIN_FILTER : uint16_t
{
	NEAREST_NEIGHBOR = GL_NEAREST,
	LINEAR = GL_LINEAR,
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};
enum class TEXTURE_MAG_FILTER : uint16_t
{
	NEAREST_NEIGHBOR = GL_NEAREST,
	LINEAR = GL_LINEAR
};
enum class TEXTURE_TYPE : uint16_t
{
	REGULAR = GL_TEXTURE_2D,
	CUBE_MAP = GL_TEXTURE_CUBE_MAP
};

struct GLACIER_API TextureParameters
{
	TEXTURE_TYPE type = TEXTURE_TYPE::REGULAR;
	TEXTURE_MIN_FILTER min_filter = TEXTURE_MIN_FILTER::LINEAR;
	TEXTURE_MAG_FILTER mag_filter = TEXTURE_MAG_FILTER::LINEAR;
	TEXTURE_WRAP wrap_s = TEXTURE_WRAP::REPEAT;
	TEXTURE_WRAP wrap_t = TEXTURE_WRAP::REPEAT;
	TEXTURE_WRAP wrap_r = TEXTURE_WRAP::REPEAT;

	TextureParameters(const TEXTURE_MIN_FILTER& min_filter = TEXTURE_MIN_FILTER::LINEAR_MIPMAP_LINEAR, const TEXTURE_MAG_FILTER& mag_filter = TEXTURE_MAG_FILTER::LINEAR,
		const TEXTURE_WRAP& wrap_s = TEXTURE_WRAP::REPEAT, const TEXTURE_WRAP& wrap_t = TEXTURE_WRAP::REPEAT, const TEXTURE_WRAP& wrap_r = TEXTURE_WRAP::REPEAT)
		: type(TEXTURE_TYPE::REGULAR), min_filter(min_filter), mag_filter(mag_filter), wrap_s(wrap_s), wrap_t(wrap_t), wrap_r(wrap_r)
	{}
};

class GLACIER_API Texture
{
protected:
	Texture(const std::string& file_name, const TextureParameters& tex_params);
	Texture(const std::array<std::string, 6>& file_paths, const TextureParameters& tex_params);
	Texture(const glm::vec4& color);
	Texture(const int width, const int height, const uint32_t num_channels, const uint8_t* data, const TextureParameters& tex_params);

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&& o) noexcept;
	Texture& operator=(Texture&& o);
	virtual ~Texture();

public:
	virtual void Bind() const = 0;

	const int GetWidth() const;
	const int GetHeight() const;
	const int GetNumChannels() const;
	const TextureParameters& GetTextureParameters() const;
	const std::string& GetFilePath() const;

	virtual void SetTextureWrapS(TEXTURE_WRAP wrap) = 0;
	virtual void SetTextureWrapT(TEXTURE_WRAP wrap) = 0;
	virtual void SetTextureWrapR(TEXTURE_WRAP wrap) = 0;
	virtual void SetTextureMinFilter(TEXTURE_MIN_FILTER filter) = 0;
	virtual void SetTextureMagFilter(TEXTURE_MAG_FILTER filter) = 0;

private:
	virtual void load_gpu_data() = 0;

	friend class TextureAtt;

protected:
	int width;
	int height;
	int channels;
	TextureParameters tex_params;
	uint8_t* img;
	std::string file_path;
};

#endif _TEXTURE