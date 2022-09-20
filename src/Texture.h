#ifndef _TEXTURE
#define _TEXTURE

enum class TEXTURE_WRAP
{
	CLAMP_TO_EDGE, 
	CLAMP_TO_BORDER, 
	MIRRORED_REPEAT, 
	REPEAT, 
	MIRROR_CLAMP_TO_EDGE
};

class Texture
{
public:
	Texture(const std::string& file_name);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&& o) noexcept;
	Texture& operator=(Texture&& o);
	~Texture();

	operator const GLuint&() const { return id; }

	void Bind() const;

	const GLuint& GetID() const;

	const int& GetWidth() const;
	const int& GetHeight() const;
	const int& GetNumChannels() const;

private:
	static void load_GPU_data(Texture& tex);

	friend class TextureLoader;

private:
	GLuint id;
	int width;
	int height;
	int channels;
	uint8_t* img;
};

#endif _TEXTURE