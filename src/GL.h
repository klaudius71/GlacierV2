#ifndef _GL
#define _GL

#if GLACIER_OPENGL

class Window;

class GL
{
public:
	static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
	static void InitializeOpenGL(const Window& window);
};

#endif

#endif
