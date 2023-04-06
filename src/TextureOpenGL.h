#ifndef _TEXTURE_OPENGL
#define _TEXTURE_OPENGL

#include "Texture.h"

class GLACIER_API TextureOpenGL : public Texture
{
public:
	template<typename... Args>
	TextureOpenGL(Args&&... args)
		: Texture(std::forward<Args>(args)...), id(0)
	{}
	TextureOpenGL(const TextureOpenGL&) = delete;
	TextureOpenGL& operator=(const TextureOpenGL&) = delete;
	TextureOpenGL(TextureOpenGL&& o) noexcept;
	TextureOpenGL& operator=(TextureOpenGL&& o);
	~TextureOpenGL();

	virtual void Bind(const uint32_t index) const override;
	void Unbind() const;

	operator const GLuint () const { return id; }
	const GLuint GetID() const;

	virtual void SetTextureWrapS(TEXTURE_WRAP wrap) override;
	virtual void SetTextureWrapT(TEXTURE_WRAP wrap) override;
	virtual void SetTextureWrapR(TEXTURE_WRAP wrap) override;
	virtual void SetTextureMinFilter(TEXTURE_MIN_FILTER filter) override;
	virtual void SetTextureMagFilter(TEXTURE_MAG_FILTER filter) override;

private:
	GLuint id;

	virtual void load_gpu_data() override;
};

#endif