#ifndef _FRAMEBUFFER
#define _FRAMEBUFFER

class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&&);
	Framebuffer& operator=(Framebuffer&&);
	~Framebuffer();

	void Bind() const;
	const GLuint& GetFBO() const;

private:
	GLuint fbo;
};

#endif _FRAMEBUFFER