#include "gpch.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer()
	: fbo(0)
{
	glGenFramebuffers(1, &fbo);
}
Framebuffer::Framebuffer(Framebuffer&& o)
	: fbo(o.fbo)
{
	o.fbo = 0;
}
Framebuffer& Framebuffer::operator=(Framebuffer&& o)
{
	fbo = o.fbo;
	o.fbo = 0;
	return *this;
}
Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

const GLuint& Framebuffer::GetFBO() const
{
	return fbo;
}
