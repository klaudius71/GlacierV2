#ifndef _FRAMEBUFFER
#define _FRAMEBUFFER

#include "GlacierCore.h"

class GLACIER_API Framebuffer
{
public:
	Framebuffer(const int& width = 1280, const int& height = 720);
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&& o);
	Framebuffer& operator=(Framebuffer&& o);
	~Framebuffer();

	void Bind() const;
	void Clear() const;
	void Unbind() const;
	const GLuint& GetFBO() const;
	const GLuint& GetTexID() const;
	const glm::ivec2& GetSize() const;

	void Resize(const int& width, const int& height);

private:
	GLuint fbo;
	GLuint tex_id;
	GLuint rbo;
	glm::ivec2 size;
};

#endif _FRAMEBUFFER