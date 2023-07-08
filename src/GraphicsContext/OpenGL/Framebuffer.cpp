#include "gpch.h"
#include "Framebuffer.h"
#include "Application.h"
#include "Window.h"

Framebuffer::Framebuffer(const int& width, const int& height)
	: fbo(0), tex_id(0), size(width, height)
{
#if GLACIER_OPENGL
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
}
Framebuffer::Framebuffer(Framebuffer&& o)
	: fbo(o.fbo), tex_id(o.tex_id), size(o.size)
{
	o.fbo = 0;
	o.tex_id = 0;
	o.size = { -1, -1 };
}
Framebuffer& Framebuffer::operator=(Framebuffer&& o)
{
	fbo = o.fbo;
	tex_id = o.tex_id;
	size = o.size;
	o.fbo = 0;
	o.tex_id = 0;
	o.size = { -1, -1 };
	return *this;
}
Framebuffer::~Framebuffer()
{
#if GLACIER_OPENGL
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &tex_id);
#endif
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}
void Framebuffer::Clear() const
{
	Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	Unbind();
}
void Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
const GLuint& Framebuffer::GetFBO() const
{
	return fbo;
}
const GLuint& Framebuffer::GetTexID() const
{
	return tex_id;
}
const glm::ivec2& Framebuffer::GetSize() const
{
	return size;
}

void Framebuffer::Resize(const int& width, const int& height)
{
	size = { width, height };

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
